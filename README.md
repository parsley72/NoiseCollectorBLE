# Notice: Repository Deprecation
This repository is deprecated and no longer actively maintained. It contains outdated code examples or practices that do not align with current MongoDB best practices. While the repository remains accessible for reference purposes, we strongly discourage its use in production environments.
Users should be aware that this repository will not receive any further updates, bug fixes, or security patches. This code may expose you to security vulnerabilities, compatibility issues with current MongoDB versions, and potential performance problems. Any implementation based on this repository is at the user's own risk.
For up-to-date resources, please refer to the [MongoDB Developer Center](https://mongodb.com/developer).

# NoiseCollectorBLE

## This Repo

This repository contains code for a C++ program that collects noise data from Bluetooth Low Energy sensors.  The code is
meant for a Raspberry Pi 4B running Raspberry Pi OS bookworm (based on Debian version 12).

## Edit and run

This code uses g++ 12.2 and CMake. I have used Emacs (with eglot) to edit, but you should survive with Visual Studio
Code.  You can build the project using the following commands:

```console
mkdir build
cmake -S . -B build
cmake --build build
./build/NoiseCollectorBLE
```

## License

This project is licensed under the terms of the [Apache license 2.0](./LICENSE.txt).

## Author

Jorge D. Ortiz-Fuentes, 2023

## Resources

- [▶️ MongoDB's YouTube channel](https://www.youtube.com/c/MongoDBofficial)
- [🙋🏻‍♂️ Jorge's Mastodon](https://fosstodon.org/@jdortiz)
- [🙋🏻‍♂️ Jorge's Twitter](https://twitter.com/jdortiz)
- [🧑🏻‍💻 Jorge's LinkedIn](https://www.linkedin.com/in/jorgeortiz/)
