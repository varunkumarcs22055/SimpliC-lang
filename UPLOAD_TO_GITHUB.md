# 🚀 Upload SimpliC to GitHub - Quick Guide

## Step 1: Initialize Git Repository

```powershell
git init
git add .
git commit -m "Initial commit: SimpliC Programming Language"
```

## Step 2: Create GitHub Repository

1. Go to https://github.com/new
2. Repository name: `SimpliC`
3. Description: `A simple educational programming language for learning Data Structures and Algorithms`
4. Choose **Public**
5. **DO NOT** check "Initialize with README"
6. Click **Create repository**

## Step 3: Push to GitHub

Replace `YOUR_USERNAME` with your GitHub username:

```powershell
git remote add origin https://github.com/YOUR_USERNAME/SimpliC.git
git branch -M main
git push -u origin main
```

## Step 4: Add Topics (Optional but Recommended)

On your GitHub repository page, click "Add topics" and add:
- `programming-language`
- `interpreter`
- `educational`
- `data-structures`
- `algorithms`
- `cpp`
- `learning`
- `dsa`

## ✅ What's Included

- **Source Code**: main.cpp, interpreter.cpp, dsa.cpp, value_impl.cpp
- **Headers**: simplic.hpp, dsa.hpp
- **Documentation**: README.md, LICENSE, CONTRIBUTING.md
- **Configuration**: .gitignore

## ❌ What's Excluded (.gitignore)

- Compiled executables (*.exe)
- DLL files (*.dll)
- SimpliC programs (*.simplic)
- IDE/Editor config files

## 🎯 Done!

Your SimpliC project is now on GitHub and ready to share! 🎉

Users can clone and compile:
```bash
git clone https://github.com/YOUR_USERNAME/SimpliC.git
cd SimpliC
g++ -std=c++11 -o simplic main.cpp interpreter.cpp dsa.cpp value_impl.cpp
```
