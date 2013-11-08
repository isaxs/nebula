#ifndef NODE_H
#define NODE_H

#include <iostream>
#include <iomanip>
#include <cmath>

#include <QObject>
#include <QString>
#include <QDebug>
#include <QElapsedTimer>

#include <CL/opencl.h>

#include "contextcl.h"
#include "miniarray.h"
#include "matrix.h"
#include "tools.h"

class SearchNode {
    /* This class represents a node in the "search octtree" data structure. It is used in order to create bricks for the GPU octtree. */
    public:
        SearchNode();
        SearchNode(SearchNode * parent, double * extent);
        ~SearchNode();

        void setOpenCLContext(OpenCLContext *context);
        void clearChildren();
        void clearPoints();
        void insert(float * point);
        void split();
        void print();
        void weighSamples(float * sample, double * sample_extent, float * sum_w, float * sum_wu, float p, float search_radius);
        bool isIntersected(double * sample_extent);

        /* gets and sets */
        void setParent(SearchNode * parent);

        int getBrick(
            MiniArray<double> * brick_extent,
            float search_radius,
            unsigned int brick_outer_dimension,
            unsigned int level,
            cl_mem * items_cl,
            cl_mem * pool_cl,
            cl_kernel * voxelize_kernel,
            int * method,
            unsigned int brick_counter,
            unsigned int brick_pool_power);
        
        void getData(double *brick_extent,
                     float * point_data,
                     size_t *accumulated_points,
                     float search_radius);
        
        
        float getIDW(float * sample, float p, float search_radius);
        unsigned int getLevel();
        unsigned int getOctant(float * point, bool * isOutofBounds);
        double * getExtent();

    private:
        void getIntersectedItems(MiniArray<double> * effective_extent, unsigned int * item_counter, cl_mem * items);
        void getIntersectedItems(MiniArray<double> * effective_extent, size_t * accumulated_points, float * point_data);
        SearchNode * parent;
        SearchNode ** children;
        float * points;
        MiniArray<double> extent;

        float distance(float * a, float * b);
//        void writeLog(QString str);

        unsigned int level;
        unsigned int n_children;
        unsigned int n_points;

        bool isEmpty;
        bool isMsd;
        cl_int err;

        OpenCLContext * context;
};
#endif
