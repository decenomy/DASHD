pipeline {

    agent any

    stages {

        stage("depends") {

            steps {
                echo 'building depends ...'
                sh 'cd depends'
                sh 'make -j 12 HOST=x86_64-pc-linux-gnu'
                sh 'make -j 12 HOST=x86_64-w64-mingw32'
                sh 'mkdir SDKs'
                sh 'cd SDKs'
                sh 'wget https://github.com/phracker/MacOSX-SDKs/releases/download/10.15/MacOSX10.11.sdk.tar.xz'
                sh 'tar -xf MacOSX10.11.sdk.tar.xz'
                sh 'cd ..'
                sh 'make -j 12 HOST=x86_64-apple-darwin14'
                sh 'cd ..'
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