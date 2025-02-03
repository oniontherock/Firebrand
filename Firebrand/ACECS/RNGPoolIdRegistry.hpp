#ifndef __RNG_POOL_ID_REGISTRY_H__
#define __RNG_POOL_ID_REGISTRY_H__

// note that the Ids for pools are the ids within their template,
// so the id for the first pool in float pools can be 0, and the first pool in the int pools can also be 0

// pool that contains values between 0.0 and 1.0
#define PROBABILITY_POOL_ID 0
// pool that contains a random value from 0 to UINT16_MAX
#define UINT_POOL_ID 0
// pool that contains a random value from -1 to 1 as an int
#define INT_SIGN_POOL_ID 0

#endif