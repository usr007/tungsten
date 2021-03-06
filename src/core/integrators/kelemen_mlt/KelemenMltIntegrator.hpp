#ifndef KELEMENMLTINTEGRATOR_HPP_
#define KELEMENMLTINTEGRATOR_HPP_

#include "KelemenMltSettings.hpp"
#include "KelemenMltTracer.hpp"

#include "integrators/Integrator.hpp"
#include "integrators/ImageTile.hpp"

#include "sampling/PathSampleGenerator.hpp"
#include "sampling/UniformSampler.hpp"

#include "thread/TaskGroup.hpp"

#include "math/MathUtil.hpp"

#include <thread>
#include <memory>
#include <vector>
#include <atomic>

namespace Tungsten {

class KelemenMltIntegrator : public Integrator
{
    struct PathCandidate
    {
        uint64 state;
        double luminanceSum;
        float luminance;
    };

    KelemenMltSettings _settings;

    std::shared_ptr<TaskGroup> _group;

    uint32 _w;
    uint32 _h;

    UniformSampler _sampler;
    std::vector<std::unique_ptr<KelemenMltTracer>> _tracers;

    bool _chainsLaunched;
    double _luminanceScale;
    std::unique_ptr<PathCandidate[]> _pathCandidates;

    virtual void saveState(OutputStreamHandle &out) override;
    virtual void loadState(InputStreamHandle &in) override;

    void traceSamplePool(uint32 taskId, uint32 numSubTasks, uint32 threadId);
    void runSampleChain(uint32 taskId, uint32 numSubTasks, uint32 threadId);

    void selectSeedPaths();

public:
    KelemenMltIntegrator();

    virtual void fromJson(const rapidjson::Value &v, const Scene &scene) override;
    virtual rapidjson::Value toJson(Allocator &allocator) const override;

    virtual void prepareForRender(TraceableScene &scene, uint32 seed) override;
    virtual void teardownAfterRender() override;

    virtual void startRender(std::function<void()> completionCallback) override;
    virtual void waitForCompletion() override;
    virtual void abortRender() override;
};

}

#endif /* KELEMENMLTINTEGRATOR_HPP_ */
