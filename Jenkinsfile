pipeline {

    agent any

    stages {

        stage("depends") {

            steps {
                echo 'building depends'
            }
        }

        stage("build_linux") {

            steps {
                echo 'building linux'
            }
        }

        stage("deploy_linux") {

            steps {
                echo 'deploy linux'
            }
        }
    }
}