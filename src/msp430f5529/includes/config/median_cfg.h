#ifndef INCLUDES_CONFIG_MEDIAN_CFG_H_
#define INCLUDES_CONFIG_MEDIAN_CFG_H_

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct sMedianNode
    {
        int value;                     //sample value
        struct sMedianNode *nextAge;   //pointer to next oldest value
        struct sMedianNode *nextValue; //pointer to next smallest value
        struct sMedianNode *prevValue; //pointer to previous smallest value
    } sMedianNode_t;

    typedef struct
    {
        unsigned int numNodes;       //median node buffer length
        sMedianNode_t *medianBuffer; //median node buffer
        sMedianNode_t *ageHead;      //pointer to oldest value
        sMedianNode_t *valueHead;    //pointer to smallest value
        sMedianNode_t *medianHead;   //pointer to median value
    } sMedianFilter_t;

    void Init_Median_Filter(void);
    int MEDIANFILTER_Init(sMedianFilter_t *medianFilter);
    int MEDIANFILTER_Insert(sMedianFilter_t *medianFilter, int sample);

#ifdef __cplusplus
}
#endif
#endif /* INCLUDES_CONFIG_MEDIAN_CFG_H_ */
