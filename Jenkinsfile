pipeline {

    agent any

    stages {

        stage("depends") {

            steps {
                echo 'building depends ...'
                sh '''#!/bin/bash
                    cd depends
                    make -j $(nproc) HOST=x86_64-pc-linux-gnu
                    make -j $(nproc) HOST=x86_64-w64-mingw32'
                    rm -rf SDKs
                    mkdir SDKs
                    cd SDKs
                    wget -nc https://github.com/phracker/MacOSX-SDKs/releases/download/10.15/MacOSX10.11.sdk.tar.xz
                    tar -xf MacOSX10.11.sdk.tar.xz
                    rm MacOSX10.11.sdk.tar.xz
                    cd ..
                    make -j $(nproc) HOST=x86_64-apple-darwin14
                '''
            }
        }

        stage("build_linux") {

            steps {
                echo 'building linux ...'
            }
        }

        stage("test_linux") {
            when {
                expression {
                    BRANCH_NAME == 'develop'
                }
            }
            steps {
                echo 'testing linux ...'
            }
        }

        stage("deploy_linux") {

            steps {
                echo 'deploy linux ...'
            }
        }
    }
    // post {
    //     always {

    //     }
    //     success {

    //     }
    //     failure {

    //     }
    // }
}