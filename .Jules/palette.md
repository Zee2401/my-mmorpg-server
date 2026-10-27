## 2026-03-31 - Empty State Navigation & Tab Accessibility in React Native
**Learning:** In tab-based UI screens, empty states in sub-tabs (such as an empty inventory) benefit from direct call-to-action buttons that programmatically switch focus back to the primary tab (e.g. Catalog Store) rather than requiring manual tab navigation.
**Action:** When adding empty state views to tabbed screens, always include a CTA button that redirects to the primary active tab with appropriate accessibility labels (`accessibilityRole="tab"` and `accessibilityRole="button"`).
