#ifndef RETAIL_APPMAN_LAYOUT_H
#define RETAIL_APPMAN_LAYOUT_H

/* Retail cAppMan object offsets (dcd10 @ 0x217CD0 region). */
enum RetailAppManOffset {
    kAppManNextModule = 4,
    kAppManArgc = 8,
    kAppManTickBudget = 16, /* cAppMan_run stores 60 (0x3c) */
    kAppManModuleTable = 0x5c,
};

/* Retail cAppMan_mainLoop @ 0x217F00: calls module fn @ table+0x28, then SYNCTASK_run (0x3E5928). */

#endif /* RETAIL_APPMAN_LAYOUT_H */
