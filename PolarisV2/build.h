#pragma once

#define POLARIS_VER "v0.4.0"
#ifdef _PROD
#define POLARIS_ID "prod"
#elif _BETA
#define POLARIS_ID "beta"
#else
#define POLARIS_ID "local"
#endif