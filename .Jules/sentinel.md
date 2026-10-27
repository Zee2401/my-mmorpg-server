# Sentinel's Journal

Critical security learnings and codebase-specific security constraints.

## 2025-05-20 - Missing Authorization Check on Quest Completion
**Vulnerability:** `completeQuest(questId)` in `GameContext` previously awarded XP and Harmony Gems without verifying whether the player met the quest's `requiredLevel`. Players could invoke `completeQuest` directly or interactively for high-level quests in unlocked regions to claim rewards prematurely.
**Learning:** In stateful React contexts or game managers, state transition functions must enforce domain authorization policies (such as level requirements or prerequisites) internally rather than relying solely on UI component level restrictions.
**Prevention:** Always perform authorization and prerequisite validation directly inside domain state mutation handlers (`GameContext` / store actions) prior to applying state updates or granting digital assets/rewards.
