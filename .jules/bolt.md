## 2025-05-18 - Rhythm Battle Game Loop & Event Listener Churn
**Learning:** Adding state variables like `notes` or frame tick triggers into `useEffect` event listener dependency arrays causes event listeners (e.g., `window.keydown`) to be torn down and re-bound on every animation/timer tick (100ms in rhythm games). This leads to garbage collection pressure and lost key inputs.
**Action:** Use `useRef` for fast-changing loop state inside keydown/event handlers, allowing the event listener to be registered once cleanly without re-binding overhead.
