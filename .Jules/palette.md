## 2025-05-18 - React Native Item Card Purchase Button Accessibility
**Learning:** In React Native screens with item cards, purchase buttons containing only an icon and raw number string (`{item.price}`) cause screen readers to announce disconnected numbers without item name context.
**Action:** Always provide explicit `accessibilityRole="button"`, `accessibilityLabel={`Buy ${item.name} for ${item.price} Harmony Gems`}`, and `accessibilityHint` on purchase touchables.
