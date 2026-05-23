#!/usr/bin/env python3
"""Generate src/mem/units/<key>.c from *_ps2.s stubs (retail .word embedding)."""

from __future__ import annotations

import argparse
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
UNITS = ROOT / "src/mem/units"
MEM = ROOT / "src/mem"
DCD10 = ROOT / "src/dcd10"
LATE = ROOT / "src/late"
VIS = ROOT / "src/visualfx"
EARLY = ROOT / "src/early"

sys.path.insert(0, str(ROOT / "tools"))
from ps2_s_to_retail_asm import fn_declaration, retail_asm_from_ps2  # noqa: E402

# Boot / runtime critical path (expand over time).
BOOT_KEYS = [
    "rom21_SYNCTASK_init",
    "rom21_func_003E6448",
    "rom21_func_0036CBF8",
    "rom21_func_0036CCB8",
    "rom21_func_0036CE00",
    "rom21_func_0036CE28",
    "rom21_func_0036D008",
    "rom21_func_0036D318",
    "rom21_func_0036D3D8",
    "rom21_func_0036D3E8",
    "rom21_func_0036CFA8",
    "rom21_func_0036D020",
    "rom21_func_0036D1F0",
    "rom21_func_0036D400",
    "rom21_func_0036D428",
    "rom21_func_0036D500",
    "rom21_func_0034FC78",
    "rom21_func_00370058",
    "rom21_SYNCTASK_add",
    "rom21_SYNCTASK_del",
    "rom21_SYNCTASK_run",
    "rom21_MUTEX_lock",
    "rom21_MUTEX_unlock",
    "rom21_THREAD_yieldticks",
    "dcd10_cAppMan_setNextModule",
    "dcd10_cAppMan_run",
    "dcd10_cAppMan_mainLoop",
    "rom21_cInputMap_init",
    "rom21_cInputMap_loadMapFile",
    "rom21_cInputMap_compileMap",
    "rom21_cUIEngine_loadFile",
    "rom21_cUIEngine_addScreenByHashName",
    "early_cFEStateTitle_onCreateScreen",
    "early_cFELocale_addFile",
    "early_cSSXApp_cSSXApp",
    "early_cSSXApp_init",
    "early_cSSXApp_initLocale",
    "early_cSSXApp_loadInputMap",
    "early_cSSXApp_startGameLoad",
    "early_cSSXApp_initload",
    "early_cSSXApp_preUpdate",
    "early_cSSXApp_flush",
    "early_cSSXApp_purge",
    "early_cSSXApp_loadexecpurge",
    "early_cSSXApp_parseCommandLine",
    "early_cSSXApp_timerCallback",
    "early_cSSXApp__cSSXApp",
    "early_cGame_load",
    "early_cGame_loadTrack",
    "rom21_cWorld_cWorld",
    "rom21_cWorld_resetMap",
    "rom21_cWorldView_getNumSections",
    "rom21_cWorldView_isSectionLoaded",
    "rom21_cWorldCache_init",
    "rom21_cWorldCache_activateSectionMem",
    "rom21_cWorldCache_updatePages",
    "rom21_cWorldCacheTable_cWorldCacheTable",
    "rom21_cWorldBlockAllocator_init",
    "rom21_cWorldMemoryMan_activateSectionMem",
    "rom21_cStreamMan_cStreamMan",
    "early_cGame_exit",
    "early_cGameViewMan_cGameViewMan",
    "early_cGameViewMan_updateAll",
    "early_cGameModeMan_initGameMode",
    "early_cGameModeMan_getGM",
    "early_cGameModeMan_restartHeat",
    "early_cGameComm_construct",
    "early_cGameComm_syncGame",
    "early_cGameLoadScreen_loadTexture",
    "rom21_cWorldSphTree_cWorldSphTree",
    "rom21_cWorldCache_addBxStreamDataTest",
    "rom21_cPSPGraphicsMan_NewBindTexID",
    "rom21_cPSPGraphicsMan_NewNonBindTexID",
    "early_cRider_initOnce",
    "early_cRider_cRider",
    "early_cRider_getMass",
    "early_cRider_addFocusBox",
    "early_cRider_addRiderMetrix",
    "early_cRider_updateOrientationImplicit",
    "early_cRiderMetrix_linkToRider",
    "rom21_cRiderSphereTree_cRiderSphereTree",
    "early_cAI_cAI",
    "early_cAI_initComputerRiders",
    "early_cAI_initMissionRiders",
    "early_cAI_InitPlayers",
    "early_cAirPredictor_reset",
    "early_cAirPredictor_initLaunch",
    "early_cAirPredictor_startLaunchIntoAir",
    "early_cRider_doLeanPoseAdjust",
    "early_cRider_quitEvent",
    "early_cAI_loadAnims",
    "early_cAI_setAIState",
    "early_cAI_forceAIState",
    "early_cAI_initComputerActors",
    "early_cBENewRaceInterface_setGameMode",
    "dcd10_cRiderAnimBase_cRiderAnimBase",
    "dcd10_cRiderAnimBase_play",
    "dcd10_cRiderAnimBase_changeHeadingOffset",
    "dcd10_cRiderAnimBase_changeOrientationOffset",
    "dcd10_cWorldLightMan_initLightCache",
    "early_cAI_readFromReplayFrame",
    "early_cAIVisualEffectsMainMenu_cAIVisualEffectsMainMenu",
    "early_cRiderMetrix_evAutoResetSurface",
    "early_cWorldPainterMan_reset",
    "early_cWorldPainterMan_getQuery",
    "early_cWorldPainterQuery_reset",
    "early_cWorldPainterQuery_cWorldPainterQuery",
    "early_cWorldTriggerManager_cWorldTriggerManager",
    "early_cWorldTriggerManager_LoadTriggerInfo",
    "early_WORLDTRIGGERMANAGER_Init",
    "early_cSectionMan_setSky",
    "dcd10_cGameAnimMap_testResolve",
    "rom21_cMemMan_trackAlloc",
    "early_cFEStateMainMenu_onCreateScreen",
    "early_cFEStateMainMenu_onWidgetCreate",
    "early_cFEStateMainMenu_onWidgetEvent",
    "early_cFEStateMountainRoom_onCreateScreen",
    "early_cFEStateMountainRoom_onWidgetCreate",
    "early_cFEStateMountainRoom_onWidgetEvent",
    "early_cFEStateOptions_onCreateScreen",
    "early_cFEStateOptionsGame_onCreateScreen",
    "early_cFEStateOptionsGame_onWidgetEvent",
    "early_cFEStateOptionsSound_onCreateScreen",
    "early_cFEStateOptionsController_onCreateScreen",
    "early_cFEStateOptionsSaveLoad_onCreateScreen",
    "early_cFEStateOptionsSaveLoad_onWidgetCreate",
    "early_cFEStateOptionsSaveLoad_onWidgetEvent",
    "early_cFEStateTransition_onCreateScreen",
    "early_cFEStateTransition_onScreenEvent",
    "early_cFEStateBonusMaterial_onCreateScreen",
    "early_cFEStateSelectMultiplayerMode_onCreateScreen",
    "early_cFEStateEventSelect_onCreateScreen",
    "early_cFEStatePeakRoom_onCreateScreen",
    "early_cFEStatePeakRoom_onWidgetCreate",
    # cmemman_alloc/free: configure uses src/mem/cmemman_alloc*.cpp directly
    "operator_new_global",
    "operator_delete",
    "crowdrender2d_init",
    "crowd_anim2d_update",
    # Object graph (rom21 @ 0x354xxx–0x397xxx) — ObjDiff + native SKIP_ASM in src/object/
    "rom21_cListNode_isSentinel",
    "rom21_cListNode_removeFromList",
    "rom21_cObjNode_cObjNode",
    "rom21_cTypeObjNode_cTypeObjNode",
    "rom21_cSortObjNode_cSortObjNode",
    "rom21_cBucketMan_add",
    "rom21_cBucketMan_addfirst",
    "rom21_cBucketMan_first",
    "rom21_cBucketMan_next",
    "rom21_cBucketMan_init",
    "rom21_cBucketMan_purgeBucket",
    "rom21_cList_first",
    "rom21_cList_addToFront",
    "rom21_cList_addToEnd",
    "rom21_cSpline_calcLength",
    "rom21_cSpline_readFromReplayFrame",
    "rom21_func_003D5330",
    "rom21_func_003D53B8",
    "early_func_002D1BD8",
    "rom21_cEffectLink_add",
    "rom21_cMoveNode_cMoveNode",
    "rom21_cMoveNode_addModifierBlock",
    "rom21_cMoveNode_addEffectModifier",
    "rom21_cMoveNode_addSpline",
    "rom21_cMoveNode_addParticle",
    "rom21_cMoveNode_addHalo",
    "rom21_cMoveNode_addDynamicParticle",
    "rom21_cMoveNode_setupOverlapSystem",
    "rom21_func_00355420",
    "rom21_func_003554B0",
    "rom21_func_00355028",
    "rom21_func_00355550",
    "rom21_func_00355B90",
    "rom21_func_003530D0",
    "rom21_func_0035F6E8",
    "rom21_tModifierBlock_readFromReplayFrame",
    "rom21_cDebounceNode_cDebounceNode",
    "rom21_cFloatingNode_initInfo",
    "rom21_cInstanceNode_cInstanceNode",
    "rom21_cInstanceMan_copyInstance",
    "rom21_cInstanceNode_getBoundBoxInfo",
    "rom21_func_0034FB00",
    "rom21_func_00354720",
    "rom21_func_00354850",
    "rom21_func_0034FC78",
    "early_func_002D1BF0",
    "early_func_002D1CF0",
    "early_func_002D9C00",
    "early_func_001032C0",
    "rom21_cRailModifier_buildXform",
    "rom21_cFlexRailNode_addSpaceHash",
    "rom21_cFlexBridgeNode_setupGrid",
    "rom21_cAnimNode_setAnimMeshCache",
    "rom21_cDeadFadeNode_cDeadFadeNode",
    "rom21_cMultiSplineModifier_allocNodes",
    "rom21_cMultiSplineModifier_setupNodes",
    "rom21_cMultiSplineModifier_setupOverlapSystem",
    "rom21_cSplineModifier_cSplineModifier",
    "rom21_cSpring_setupNodes",
    "rom21_tModifierBlock_tModifierBlock",
    "rom21_tModifierBlock_setBoundBox",
    "rom21_tModifierBlock_setRadius",
    "early_cObjectInterface_getInstanceMan",
    "early_cReplay_restoreObject",
]

# configure ps2_asm_map key -> ps2.s path (when filename != key_ps2.s)
PS2_ALIASES: dict[str, Path] = {
    "crowdrender2d_init": VIS / "crowdrender2d_init_ps2.s",
    "crowd_anim2d_update": VIS / "crowd_anim2d_update_ps2.s",
    "cmemman_alloc": MEM / "cmemman_alloc_ps2.s",
    "cmemman_free": MEM / "cmemman_free_ps2.s",
}


def find_ps2(key: str) -> Path | None:
    if key in PS2_ALIASES and PS2_ALIASES[key].is_file():
        return PS2_ALIASES[key]
    for base in (MEM, DCD10, LATE, VIS, EARLY):
        p = base / f"{key}_ps2.s"
        if p.is_file():
            return p
    return None


def symbol_from_key(key: str) -> str:
    for prefix in ("rom21_", "dcd10_", "late_", "early_", "crowd_", "cmemman_", "operator_"):
        if key.startswith(prefix):
            return key[len(prefix) :]
    return key.split("_", 1)[-1]


def write_unit(key: str, ps2: Path, decl: str | None, force: bool = False) -> Path | None:
    _, asm_body, _ = retail_asm_from_ps2(ps2)
    out = UNITS / f"{key}.c"
    if out.is_file() and not force:
        text = out.read_text(encoding="utf-8", errors="replace")
        if "// @objdiff-matched" in text and "PS2_RETAIL_ASM_ONLY" not in text:
            return None
    parts = [
        '#include "common.h"\n',
        '#include "ps2_match.h"\n\n',
        "#ifdef SKIP_ASM\n",
    ]
    sym = symbol_from_key(key)
    if decl:
        parts.append(f"{decl}\n")
    else:
        parts.append(f"void {sym}(void);\n")
    parts.append(f"{asm_body}\n")
    parts.append("#endif\n")
    out.write_text("".join(parts), encoding="utf-8")
    return out


def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument("--boot", action="store_true", help="Generate boot-critical set")
    ap.add_argument("--force", action="store_true", help="Overwrite units even if hand-decompiled")
    ap.add_argument("keys", nargs="*", help="Explicit unit keys (e.g. rom21_SYNCTASK_init)")
    args = ap.parse_args()

    keys = list(args.keys)
    if args.boot:
        keys = BOOT_KEYS

    UNITS.mkdir(parents=True, exist_ok=True)
    written = 0
    for key in keys:
        ps2 = find_ps2(key)
        if not ps2:
            print(f"[units] skip {key}: no *_ps2.s")
            continue
        decl = None
        out = write_unit(key, ps2, decl, force=args.force)
        if out is None:
            print(f"[units] keep {key}.c (hand-decompiled, use --force to replace)")
            continue
        print(f"[units] wrote {key}.c from {ps2.name}")
        written += 1

    print(f"[units] generated {written} unit(s); run: python3 tools/batch_verify_units.py")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
