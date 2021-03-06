#ifndef WORKER_H
#define WORKER_H

//#include <QScriptEngine>
#include <QPlainTextEdit>

/* Project files */
#include "../math/qxmathlib.h"
#include "../opencl/qxopencllib.h"
#include "../file/qxfilelib.h"
#include "../svo/qxsvolib.h"

class BaseWorker : public QObject, protected OpenCLFunctions
{
        Q_OBJECT

    public:
        BaseWorker();
        ~BaseWorker();

        void setReducedPixels(Matrix<float> * reduced_pixels);
        void setSVOFile(SparseVoxelOctree * svo);

    signals:
        void finished();
        void abort();
        void changedMessageString(QString str);
        void changedGenericProgress(int value);
        void changedMemoryUsage(int value);
        void changedFormatGenericProgress(QString str);
        void changedFormatMemoryUsage(QString str);
        void changedRangeMemoryUsage(int min, int max);
        void changedRangeGenericProcess(int min, int max);
        void popup(QString title, QString text);

    public slots:
        void setSet(SeriesSet set);
        void killProcess();
        void setActiveAngle(int value);
        void setOffsetOmega(double value);
        void setOffsetKappa(double value);
        void setOffsetPhi(double value);
        void setQSpaceInfo(float suggested_search_radius_low, float suggested_search_radius_high, float suggested_q);

    protected:
        // Runtime
        bool kill_flag;

        // OpenCL
        OpenCLContext context_cl;
        cl_int err;
        cl_program program;
        bool isCLInitialized;

        // Voxelize
        SparseVoxelOctree * svo;
        float  suggested_search_radius_low;
        float  suggested_search_radius_high;
        float  suggested_q;

        // File treatment
        int active_angle;
        SeriesSet set;
        Matrix<float> * reduced_pixels;

        double offset_omega;
        double offset_kappa;
        double offset_phi;

        size_t GLOBAL_VRAM_ALLOC_MAX;
};


class VoxelizeWorker : public BaseWorker
{
        Q_OBJECT

    public:
        VoxelizeWorker();
        ~VoxelizeWorker();

    public slots:
        void process();
        void initializeCLKernel();


    signals:
        void showProgressBar(bool value);

    protected:
        cl_kernel voxelize_kernel;
        cl_kernel fill_kernel;

        unsigned int getOctIndex(unsigned int msdFlag, unsigned int dataFlag, unsigned int child);
        unsigned int getOctBrick(unsigned int poolX, unsigned int poolY, unsigned int poolZ);
};



#endif
