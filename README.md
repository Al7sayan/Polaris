# PolarisV2
They said to think back, but I think we went a bit too far.

This is the v1.0.0 codebase. A work-in-progress codebase for v1.1.0 can be found on the [https://github.com/PolarisV2/Polaris/tree/dev/](**dev branch**).

# Building
Add an __**SDK**__ (software development kit) to your Polaris instance. The required files are found in **#polaris-news** within the official Discord server; the cleaner, less messy **#fn-sdk** channel will be available at a later time.
You might need to fix your SDK the first time you set up the Polaris codebase by undefining PF_MAX in FN_CoreUObject_Classes.hpp.

# Libraries
| Library       | License       | Usage                                                     |
| ------------- | ------------- | --------------------------------------------------------- |
| MinHook       | BSD-2-Clause  | Used for hooking directly into Fortnite.                  |
| Dear ImGui    | MIT           | Used for the Polaris menu.                                |
