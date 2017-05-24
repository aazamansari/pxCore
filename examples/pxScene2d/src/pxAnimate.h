/*

 pxCore Copyright 2005-2017 John Robinson

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

     http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.

*/

// pxAnimate.h

#ifndef PX_ANIMATE_H
#define PX_ANIMATE_H

#include "pxConstants.h"
class pxObject;
struct animation;

static rtString mapStatus(uint32_t status)
{
   pxConstantsAnimation::animationStatus v = (pxConstantsAnimation::animationStatus) status;
   if (v == pxConstantsAnimation::STATUS_IDLE)
   {
     return "IDLE";
   }
   else if ( v == pxConstantsAnimation::STATUS_INPROGRESS)
   {
     return "INPROGRESS";
   }
   else if ( v == pxConstantsAnimation::STATUS_CANCELLED)
   {
     return "CANCELLED";
   }
   else if ( v == pxConstantsAnimation::STATUS_ENDED)
   {
     return "ENDED";
   }
   return "UNKNOWN";
}

/**********************************************************************
 * 
 * pxAnimate
 * 
 **********************************************************************/
class pxAnimate: public rtObject
{
  public:

    pxAnimate(rtObjectRef props, uint32_t interp, pxConstantsAnimation::animationOptions type, double duration, int32_t count, rtObjectRef promise, rtRef<pxObject> animateObj);

    virtual ~pxAnimate();

    rtDeclareObject(pxAnimate, rtObject);

    // common properties of animation
    rtReadOnlyProperty(done, done, rtObjectRef);
    rtReadOnlyProperty(type, type, uint32_t);
    rtReadOnlyProperty(interp, interp, uint32_t);
    rtReadOnlyProperty(status, status, rtString);
    rtReadOnlyProperty(provduration, provduration, double);
    rtReadOnlyProperty(provcount, provcount, int32_t);
    rtReadOnlyProperty(cancelled, cancelled, bool);
    rtReadOnlyProperty(props, props, rtObjectRef);
    // property to describe the animation status of the properties mentioned in animate call
    rtReadOnlyProperty(details, details, rtObjectRef);
    rtMethodNoArgAndNoReturn("cancel", cancel);

    rtError done(rtObjectRef& v)   const { v = mDonePromise; return RT_OK;   }
    rtError type(uint32_t& v)   const { v = (uint32_t) mType; return RT_OK;   }
    rtError interp(uint32_t& v)   const { v = mInterp; return RT_OK;   }
    rtError status(rtString& v) const { v = mapStatus(mStatus); return RT_OK; };
    rtError provduration(double& v)   const { v = mProvisionedDuration; return RT_OK;   }
    rtError provcount(int32_t& v)   const { v = mProvisionedCount; return RT_OK;   }
    rtError cancelled(bool& v) const { v = mCancelled; return RT_OK; }
    rtError props(rtObjectRef& v)   const { v = mProps; return RT_OK;   }
    rtError details(rtObjectRef& v) const { v = mCurrDetails; return RT_OK; }
    rtError cancel();


    // internal public methods
    void setStatus(uint32_t v);
    // update the animation details of every parameter
    // this is invoked on every parameter update during the process of animation
    void update(const char* prop, struct animation* params, uint32_t status);

    class pxAnimationParams : public rtObject
    {
      public:
        rtDeclareObject(pxAnimate::pxAnimationParams, rtObject);
        rtReadOnlyProperty(from, from, double); 
        rtReadOnlyProperty(to, to, double); 
        rtReadOnlyProperty(duration, duration, double);
        rtReadOnlyProperty(count, count, int32_t);
        rtReadOnlyProperty(status, status, rtString);
        rtReadOnlyProperty(cancelled, cancelled, bool);
        
        rtError from(double& v)   const { v = mFrom; return RT_OK;   }
        rtError to(double& v)   const { v = (uint32_t) mTo; return RT_OK;   }
        rtError duration(double& v)   const { v = mDuration; return RT_OK; }
        rtError count(int32_t& v)   const { v = mCount; return RT_OK;   }
        rtError status(rtString& v)   const { v = mapStatus(mStatus); return RT_OK;   }
        rtError cancelled(bool& v)   const { v = mCancelled; return RT_OK;   }
   
        uint32_t mStatus;
        int32_t mCount;
        bool mCancelled;
        double mDuration;
        double mFrom;
        double mTo;
    };
  private:
    rtObjectRef mProps;
    rtObjectRef mCurrDetails;
    uint32_t mInterp;
    pxConstantsAnimation::animationOptions mType;
    double mProvisionedDuration;
    int32_t mProvisionedCount;
    bool mCancelled;
    uint32_t mStatus;
    rtObjectRef mDonePromise;
    rtRef<pxObject> mAnimatedObj;
};

typedef rtRef<pxAnimate> pxAnimateRef;
typedef rtRef<pxAnimate::pxAnimationParams> pxAnimateParamsRef;

#endif
