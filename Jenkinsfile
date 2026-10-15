pipeline {
    agent any

    environment {
        IMAGE_NAME = 'akhilesh840/train-traffic-image'
    }

    stages {
        stage('Checkout') {
            steps {
                echo 'Checking out source code from Git repository...'
                checkout scm
            }
        }

        stage('Build Docker Image') {
            steps {
                echo "Building Docker container image ${IMAGE_NAME}..."
                script {
                    if (isUnix()) {
                        sh "docker build -t ${IMAGE_NAME}:${BUILD_NUMBER} -t ${IMAGE_NAME}:latest ."
                    } else {
                        bat "docker build -t ${IMAGE_NAME}:${BUILD_NUMBER} -t ${IMAGE_NAME}:latest ."
                    }
                }
            }
        }

        stage('Test & Health Check') {
            steps {
                echo 'Validating Docker container startup and health...'
                script {
                    if (isUnix()) {
                        sh """
                            docker run -d --name train_test_run -p 8085:80 ${IMAGE_NAME}:latest
                            sleep 5
                            docker ps | grep train_test_run
                            docker stop train_test_run
                            docker rm train_test_run
                        """
                    } else {
                        bat """
                            docker run -d --name train_test_run -p 8085:80 ${IMAGE_NAME}:latest
                            timeout /t 5 /nobreak
                            docker ps
                            docker stop train_test_run
                            docker rm train_test_run
                        """
                    }
                }
            }
        }
    }

    post {
        always {
            echo 'Pipeline execution completed.'
        }
        success {
            echo 'Docker build and verification succeeded!'
        }
        failure {
            echo 'Docker build or verification failed.'
        }
    }
}
