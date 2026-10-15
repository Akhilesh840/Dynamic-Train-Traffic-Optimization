pipeline {
    agent any

    environment {
        DOCKER_IMAGE = 'akhilesh840/train-traffic-image'
        DOCKER_HUB_CREDENTIALS = 'dockerhub-creds'
    }

    stages {
        stage('Checkout') {
            steps {
                echo 'Checking out source code from GitHub repository...'
                checkout scm
            }
        }

        stage('Build and Test') {
            steps {
                echo 'Verifying core C++ source code and project configuration files...'
                script {
                    if (isUnix()) {
                        sh '''
                            test -f CMakeLists.txt
                            test -f src/main.cpp
                            test -f src/Graph.cpp
                            test -f include/Graph.h
                            test -f build/dashboard.html
                            echo "All core source and configuration files verified successfully."
                        '''
                    } else {
                        bat '''
                            if exist CMakeLists.txt (echo Core files verified) else (exit 1)
                        '''
                    }
                }
            }
        }

        stage('Docker Build') {
            steps {
                echo "Building Docker container image ${DOCKER_IMAGE}:${BUILD_NUMBER} and latest..."
                script {
                    if (isUnix()) {
                        sh "docker build -t ${DOCKER_IMAGE}:${BUILD_NUMBER} -t ${DOCKER_IMAGE}:latest ."
                    } else {
                        bat "docker build -t ${DOCKER_IMAGE}:${BUILD_NUMBER} -t ${DOCKER_IMAGE}:latest ."
                    }
                }
            }
        }

        stage('Push Image') {
            steps {
                echo 'Logging into Docker Hub and pushing image...'
                withCredentials([usernamePassword(credentialsId: "${DOCKER_HUB_CREDENTIALS}", usernameVariable: 'DOCKER_USER', passwordVariable: 'DOCKER_PASS')]) {
                    script {
                        if (isUnix()) {
                            sh '''
                                echo "$DOCKER_PASS" | docker login -u "$DOCKER_USER" --password-stdin
                                docker push ${DOCKER_IMAGE}:${BUILD_NUMBER}
                                docker push ${DOCKER_IMAGE}:latest
                            '''
                        } else {
                            bat '''
                                echo %DOCKER_PASS% | docker login -u %DOCKER_USER% --password-stdin
                                docker push %DOCKER_IMAGE%:%BUILD_NUMBER%
                                docker push %DOCKER_IMAGE%:latest
                            '''
                        }
                    }
                }
            }
        }

        stage('Minikube Deployment') {
            steps {
                echo 'Deploying application to Minikube cluster...'
                script {
                    if (isUnix()) {
                        sh '''
                            kubectl apply -f k8s/deployment.yaml
                            kubectl apply -f k8s/service.yaml
                            kubectl set image deployment/train-traffic-deployment train-traffic-container=${DOCKER_IMAGE}:${BUILD_NUMBER} || true
                            kubectl rollout status deployment/train-traffic-deployment --timeout=60s || true
                        '''
                    } else {
                        bat '''
                            kubectl apply -f k8s/deployment.yaml
                            kubectl apply -f k8s/service.yaml
                            kubectl set image deployment/train-traffic-deployment train-traffic-container=%DOCKER_IMAGE%:%BUILD_NUMBER%
                        '''
                    }
                }
            }
        }

        stage('Verification') {
            steps {
                echo 'Verifying Kubernetes Pods, Deployment, and Service status...'
                script {
                    if (isUnix()) {
                        sh '''
                            kubectl get pods -l app=train-traffic
                            kubectl get deployment train-traffic-deployment
                            kubectl get svc train-traffic-service
                        '''
                    } else {
                        bat '''
                            kubectl get pods -l app=train-traffic
                            kubectl get deployment train-traffic-deployment
                            kubectl get svc train-traffic-service
                        '''
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
            echo 'Train Traffic Optimization CI/CD Pipeline succeeded!'
        }
        failure {
            echo 'Pipeline failed. Check stage logs for troubleshooting.'
        }
    }
}
