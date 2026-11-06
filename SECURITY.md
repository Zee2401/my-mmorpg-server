# Security Policy

## Supported Versions

This project is actively maintained. Security updates and patches are provided for all released versions.

| Version | Supported          |
| ------- | ------------------ |
| 1.x     | :white_check_mark: |
| < 1.0   | :x:                |

## Reporting a Vulnerability

If you discover a security vulnerability in MusicMORPG App, please report it immediately.

### How to Report

1. **Private Disclosure**: Please do NOT open a public GitHub issue. Instead, contact the maintainer privately via email or Discord.
2. **Details to Include**:
   - Steps to reproduce the vulnerability
   - Impact assessment
   - Suggested fix (if available)
   - Your contact information

### Response Time

- Initial acknowledgment within 24-48 hours
- Regular updates on progress
- Target patch release within 7-14 days for critical vulnerabilities

### Security Focus Areas

This project handles:
- Local game state persistence via AsyncStorage
- User input for character customization
- Audio synthesis and playback

Known security considerations:
- All stored data is validated and sanitized before use
- JSON parsing from storage uses try-catch with fallback to initial state
- Numeric values are validated to prevent manipulation
- No external API calls or network requests are made

## Security Best Practices

When contributing to this project:

1. **Input Validation**: Always validate and sanitize any user input or stored data
2. **Safe Parsing**: Use try-catch blocks when parsing JSON from storage
3. **Type Safety**: Leverage TypeScript to prevent type-related vulnerabilities
4. **No Secrets**: Never commit API keys, credentials, or other secrets
5. **Dependency Security**: Regularly update dependencies to address known vulnerabilities
