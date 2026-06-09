pipeline {
    agent { dockerfile true }
    options {
        buildDiscarder(logRotator(numToKeepStr: "50"))
    }
    stages {
        stage('Ensure clean build') {
            steps {
                sh 'git clean -dfx'
            }
        }
        stage('Submodules') {
            steps {
                sh 'git submodule update --init --recursive'
            }
        }
        stage('Build') {
            steps {
                sh 'cmake -B build && cmake --build build'
            }
        }
        stage('Test') {
            steps {
                sh 'ctest --test-dir build'
            }
            post {
                always {
                    xunit (
                      tools: [ GoogleTest(pattern: 'build/testresults/*.xml') ]
                    )
                }
            }
        }
    }
}
