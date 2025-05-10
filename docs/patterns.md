# Patterns

Some supported symbols in the app.

| Symbol | Meaning |
| - | - |
| `*` | Matches **zero or more** of any characters. |
| `+` | Matches **one or more** of any characters. |
| `?` | Matches **exactly one** of any character. Can be used consecutively. |
| `_` | **Suggestion mode** - when placed at the end, it triggers prefix suggestions. |

## Notes

- The app **only** support consecutive `?` patterns.
- You **can combine** `*`, `+`, and `?` in a single pattern, but not with `_`.
- The `_` symbol **must be placed at the end** and **cannot** be used with other placeholders.
- If you type a word without any patterns and it's **not found**, the app will automatically do a **fuzzy search** (using a BK-Tree) to suggest similar words.

## Examples

- `c?t` matches "cat", "cut", etc.
- `c???t` matches "count", "coast", etc.
- `appl_` suggests "apple", "application", etc.
- `*tion` matches "motion", "station", etc.
- `ca*t` matches "cat", while "ca+t" cannot match "cat".
