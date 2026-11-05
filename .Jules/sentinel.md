# Sentinel's Journal

Critical security learnings and codebase-specific security constraints.

## 2026-11-05 - Context-Level Quest Authorization and Numeric Sanitization
**Vulnerability:** `completeQuest` allowed claiming rewards for any quest without verifying quest existence or whether the player's level met `quest.requiredLevel`. Additionally, `gainXpAndGems` accepted arbitrary inputs without bounds or finite number checks, risking infinite level-up loops and profile state corruption.
**Learning:** Client-side React Context state mutations cannot rely on UI screens or button state for authorization. In a cross-platform MMORPG, domain rules (such as level requirements for quest completion) and input boundaries must be validated and enforced directly inside Context actions.
**Prevention:** Always enforce authorization checks (e.g., verifying `player.level >= quest.requiredLevel`) and sanitize numerical inputs (finite, non-negative numbers) at the state management layer before updating state or executing loops.
