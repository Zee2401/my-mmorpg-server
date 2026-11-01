# Sentinel's Journal

Critical security learnings and codebase-specific security constraints.

## 2026-11-01 - Authorization and Numeric Input Sanitization in GameContext
**Vulnerability:** `completeQuest` permitted claiming rewards for any quest without checking player level requirements or validating quest existence. `gainXpAndGems` accepted arbitrary inputs without bounds or finite number checks, risking infinite loops and state corruption.
**Learning:** Client-side state actions in React Context must perform their own authorization checks (e.g., player level requirements) and numeric sanitization, rather than relying solely on UI presentation layers.
**Prevention:** Always enforce domain authorization rules and strictly sanitize numerical amounts (finite, non-negative, integer bounds) directly inside state mutation handlers.
