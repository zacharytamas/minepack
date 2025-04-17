# Python Development Setup

This document describes how to set up the Python development environment for this project.

## Virtual Environment Setup

The project uses Python's virtual environment (`venv`) to manage dependencies. Here's how to set it up:

1. Create a new virtual environment:

   ```bash
   python3 -m venv venv
   ```

2. Activate the virtual environment:

   - On macOS/Linux:
     ```bash
     source venv/bin/activate
     ```
   - On Windows:
     ```bash
     .\venv\Scripts\activate
     ```

3. Install dependencies:

   ```bash
   pip install -r requirements.txt
   ```

4. When you're done working, deactivate the virtual environment:
   ```bash
   deactivate
   ```

## Adding New Dependencies

When adding new Python packages to the project:

1. Make sure your virtual environment is activated
2. Install the new package:
   ```bash
   pip install package_name
   ```
3. Update requirements.txt:
   ```bash
   pip freeze > requirements.txt
   ```

## Notes

- The virtual environment directory (`venv/`) is ignored by git
- Always activate the virtual environment before running Python scripts or installing packages
- If you encounter any issues with pip, you may need to upgrade it:
  ```bash
  python -m pip install --upgrade pip
  ```
