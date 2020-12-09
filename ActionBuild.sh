sudo chmod 777 ./gradlew
sdkmanager --install "ndk;21.1.6352462" "cmake;3.10.2.4988404"
./gradlew :sdk:assembleRelease