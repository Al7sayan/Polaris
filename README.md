# PolarisV2
They said to think back, but I think we went a bit too far.

# Building
Add an __**SDK**__ (software development kit) to your Polaris instance. The required files are found in **#polaris-news**; the cleaner, less messy **#fn-sdk** channel will be available at a later time.
You might need to fix your SDK the first time you set up the Polaris codebase by undefining PF_MAX in FN_CoreUObject_Classes.hpp.

# Libraries
| Library       | License       | Usage                                                     |
| ------------- | ------------- | --------------------------------------------------------- |
| MinHook       | BSD-2-Clause  | Used for hooking directly into Fortnite.                  |
| Dear ImGui    | MIT           | Used for the Polaris menu.                                |
| nlohmann/json | MIT           | Used for saving and loading Polaris Creation Files (pcf). |
