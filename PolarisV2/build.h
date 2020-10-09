#pragma once

#define POLARIS_VER "v0.0.1"
#ifdef _PROD
#define POLARIS_ID "prod"
#elif _BETA
#define POLARIS_ID "beta"
#endif