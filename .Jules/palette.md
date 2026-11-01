## 2026-03-31 - Empty State Navigation & Tab Accessibility in React Native
**Learning:** In tab-based UI screens, empty states in sub-tabs (such as an empty inventory) benefit from direct call-to-action buttons that programmatically switch focus back to the primary tab (e.g. Catalog Store) rather than requiring manual tab navigation.
**Action:** When adding empty state views to tabbed screens, always include a CTA button that redirects to the primary active tab with appropriate accessibility labels (`accessibilityRole="tab"` and `accessibilityRole="button"`).

## 2026-04-01 - Vitality Recovery Action Feedback & State Reflection
**Learning:** In game and status dashboards, instant-recovery action buttons (e.g. Rest & Recharge) lead to user confusion if they remain visually active and clickable when stats (HP/Energy) are already at 100%. Changing the button appearance to a distinct subdued "restored" state with an explanatory message, pairing restoration with clear chime audio feedback, and providing explicit accessibility labels and hints eliminates ambiguous presses for both sighted and screen reader users.
**Action:** When designing instant-recovery or top-up actions, always evaluate whether maximum capacity is reached, reflect this state via `disabled`, `accessibilityState={{ disabled }}`, appropriate icon changes, and provide multi-sensory feedback upon execution.
