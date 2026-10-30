## 2026-03-31 - Empty State Navigation & Tab Accessibility in React Native
**Learning:** In tab-based UI screens, empty states in sub-tabs (such as an empty inventory) benefit from direct call-to-action buttons that programmatically switch focus back to the primary tab (e.g. Catalog Store) rather than requiring manual tab navigation.
**Action:** When adding empty state views to tabbed screens, always include a CTA button that redirects to the primary active tab with appropriate accessibility labels (`accessibilityRole="tab"` and `accessibilityRole="button"`).

## 2026-03-31 - Accessibility Metadata for Interactive Badges & Modals in React Native
**Learning:** In React Native hero cards and selection modals, custom interactive elements (`TouchableOpacity`) such as class selection badges, action triggers, and modal option list items lack implicit ARIA or button semantics. Without explicit `accessibilityRole="button"`, `accessibilityLabel`, `accessibilityHint`, and `accessibilityState={{ selected }}`, screen readers cannot convey selection state or trigger actions to users.
**Action:** Always annotate custom `TouchableOpacity` buttons in React Native screens with `accessibilityRole="button"`, descriptive `accessibilityLabel` & `accessibilityHint`, and `accessibilityState={{ selected }}` when representing active option states.
