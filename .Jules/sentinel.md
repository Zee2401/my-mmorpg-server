# Sentinel's Journal

Critical security learnings and codebase-specific security constraints.

## 2026-10-26 - Authorization and Numeric Input Sanitization in GameContext
**Vulnerability:** `completeQuest` allowed completing any quest and granting XP/Gems without checking if player level requirements were met. `gainXpAndGems` lacked numeric finite checks.
**Learning:** Client-side state handlers must validate domain authorization (level requirements) and sanitize numeric bounds internally, even if UI components enforce restrictions.
**Prevention:** Validate input types, finite status, non-negativity, and authorization rules at the Context state action level.
