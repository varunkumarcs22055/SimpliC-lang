# Contributing to SimpliC

Thank you for your interest in contributing to SimpliC! 🎉

## How to Contribute

### Reporting Bugs 🐛

If you find a bug, please create an issue with:
- A clear, descriptive title
- Steps to reproduce the bug
- Expected vs actual behavior
- SimpliC code that demonstrates the issue
- Your operating system and compiler version

### Suggesting Features 💡

Feature suggestions are welcome! Please:
- Check if the feature has already been requested
- Provide a clear use case
- Include example SimpliC code showing how it would work

### Code Contributions 🔧

1. **Fork the repository**
2. **Create a feature branch**
   ```bash
   git checkout -b feature/your-feature-name
   ```
3. **Make your changes**
   - Follow the existing code style
   - Add comments for complex logic
   - Test your changes thoroughly
4. **Commit your changes**
   ```bash
   git commit -m "Add: brief description of changes"
   ```
5. **Push to your fork**
   ```bash
   git push origin feature/your-feature-name
   ```
6. **Create a Pull Request**

### Code Style Guidelines

- Use C++11 standard
- Indent with 4 spaces (no tabs)
- Use descriptive variable names
- Add comments for non-obvious code
- Keep functions focused and concise

### Testing

Before submitting a PR:
- Compile without errors or warnings
- Test with the example programs
- Add new test cases if adding features

### Areas for Contribution

- 🔧 Bug fixes
- ✨ New language features
- 📚 Documentation improvements
- 🧪 Additional test cases
- 🎯 Performance optimizations
- 🌍 Cross-platform support

## Development Setup

1. Clone the repository
2. Compile the project:
   ```bash
   g++ -std=c++11 -o simplic.exe main.cpp interpreter.cpp dsa.cpp value_impl.cpp
   ```
3. Test with example programs:
   ```bash
   ./simplic.exe AddtwoNumbers.simplic
   ```

## Questions?

Feel free to open an issue for any questions or clarifications!

---

Thank you for making SimpliC better! 🚀
