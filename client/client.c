#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <netinet/in.h>
#include <netdb.h>
#define PORT 8000

float rearLeft[] = {0.94, 0.95, 0.96, 0.96, 0.97, 0.98, 0.98, 0.99, 0.99, 1, 1, 1, 1.02, 1.02, 1.03, 1.02, 1.03, 1.04, 1.04, 1.05, 1.05, 1.06, 1.06, 1.07, 1.07, 1.08, 1.09, 1.09, 1.09, 1.09, 1.1, 1.1, 1.11, 1.11, 1.11, 1.11, 1.12, 1.12, 1.13, 1.13, 1.13, 1.14, 1.14, 1.13, 1.15, 1.15, 1.15, 1.15, 1.16, 1.16, 1.16, 1.17, 1.17, 1.16, 1.17, 1.17, 1.17, 1.18, 1.18, 1.18, 1.19, 1.19, 1.19, 1.19, 1.19, 1.2, 1.2, 1.2, 1.2, 1.2, 1.2, 1.21, 1.21, 1.21, 1.21, 1.22, 1.21, 1.21, 1.22, 1.22, 1.22, 1.23, 1.21, 1.23, 1.23, 1.23, 1.23, 1.23, 1.23, 1.23, 1.23, 1.23, 1.24, 1.24, 1.24, 1.24, 1.24, 1.24, 1.25, 1.24, 1.25, 1.25, 1.25, 1.25, 1.25, 1.25, 1.26, 1.26, 1.26, 1.26, 1.26, 1.27, 1.26, 1.26, 1.26, 1.26, 1.27, 1.27, 1.27, 1.27, 1.27, 1.26, 1.27, 1.27, 1.28, 1.28, 1.28, 1.28, 1.29, 1.28, 1.28, 1.28, 1.28, 1.29, 1.29, 1.29, 1.29, 1.29, 1.29, 1.29, 1.29, 1.29, 1.29, 1.29, 1.29, 1.29, 1.29, 1.29, 1.29, 1.29, 1.29, 1.29, 1.29, 1.29, 1.3, 1.3, 1.29, 1.3, 1.3, 1.29, 1.3, 1.3, 1.3, 1.3, 1.3, 1.3, 1.3, 1.3, 1.3, 1.31, 1.3, 1.3, 1.3, 1.31, 1.31, 1.3, 1.31, 1.3, 1.3, 1.31, 1.31, 1.31, 1.31, 1.31, 1.31, 1.31, 1.31, 1.31, 1.31, 1.31, 1.31, 1.31, 1.31, 1.31, 1.31, 1.31, 1.31, 1.31, 1.31, 1.31, 1.32, 1.31, 1.31, 1.31, 1.32, 1.32, 1.31, 1.31, 1.31, 1.31, 1.31, 1.32, 1.32, 1.32, 1.32, 1.32, 1.32, 1.32, 1.32, 1.32, 1.32, 1.32, 1.32, 1.32, 1.32, 1.32, 1.31, 1.32, 1.31, 1.32, 1.32, 1.32, 1.32, 1.32, 1.33, 1.33, 1.33, 1.33, 1.33, 1.33, 1.33, 1.33, 1.33, 1.33, 1.33, 1.33, 1.33, 1.33, 1.33, 1.33, 1.33, 1.33, 1.33, 1.33, 1.33};
float rearRight[] = {0.54, 0.55, 0.55, 0.56, 0.56, 0.56, 0.56, 0.57, 0.57, 0.57, 0.57, 0.58, 0.58, 0.58, 0.59, 0.59, 0.59, 0.6, 0.6, 0.6, 0.61, 0.61, 0.6, 0.61, 0.61, 0.62, 0.62, 0.62, 0.63, 0.63, 0.63, 0.63, 0.63, 0.64, 0.64, 0.64, 0.65, 0.65, 0.65, 0.65, 0.65, 0.66, 0.66, 0.66, 0.66, 0.66, 0.66, 0.67, 0.67, 0.67, 0.67, 0.68, 0.68, 0.68, 0.68, 0.68, 0.69, 0.69, 0.69, 0.69, 0.69, 0.7, 0.69, 0.7, 0.7, 0.71, 0.71, 0.71, 0.71, 0.71, 0.71, 0.71, 0.71, 0.72, 0.71, 0.72, 0.72, 0.72, 0.72, 0.72, 0.73, 0.73, 0.73, 0.73, 0.74, 0.73, 0.73, 0.73, 0.74, 0.74, 0.73, 0.74, 0.74, 0.75, 0.74, 0.74, 0.75, 0.75, 0.75, 0.75, 0.75, 0.75, 0.75, 0.76, 0.75, 0.75, 0.76, 0.76, 0.76, 0.76, 0.76, 0.76, 0.77, 0.77, 0.76, 0.77, 0.77, 0.77, 0.77, 0.77, 0.77, 0.78, 0.78, 0.77, 0.78, 0.78, 0.78, 0.78, 0.78, 0.78, 0.79, 0.78, 0.79, 0.79, 0.79, 0.79, 0.79, 0.79, 0.79, 0.79, 0.79, 0.79, 0.8, 0.79, 0.8, 0.79, 0.79, 0.79, 0.8, 0.79, 0.8, 0.8, 0.8, 0.8, 0.8, 0.8, 0.8, 0.8, 0.8, 0.8, 0.8, 0.8, 0.8, 0.81, 0.81, 0.81, 0.81, 0.81, 0.81, 0.81, 0.81, 0.81, 0.81, 0.81, 0.81, 0.81, 0.81, 0.81, 0.82, 0.81, 0.81, 0.81, 0.81, 0.82, 0.82, 0.82, 0.82, 0.82, 0.81, 0.82, 0.81, 0.81, 0.82, 0.81, 0.81, 0.82, 0.82, 0.82, 0.82, 0.82, 0.82, 0.82, 0.82, 0.81, 0.82, 0.82, 0.82, 0.82, 0.82, 0.82, 0.83, 0.82, 0.82, 0.82, 0.82, 0.83, 0.83, 0.83, 0.82, 0.83, 0.82, 0.82, 0.82, 0.83, 0.82, 0.83, 0.81, 0.82, 0.83, 0.83, 0.83, 0.83, 0.83, 0.83, 0.83, 0.83, 0.83, 0.84, 0.83, 0.83, 0.84, 0.83, 0.84, 0.84, 0.84, 0.84, 0.84, 0.84, 0.84, 0.84, 0.84, 0.84, 0.84, 0.85, 0.85};
float frontLeft[] = {0.51, 0.52, 0.51, 0.52, 0.53, 0.53, 0.52, 0.53, 0.53, 0.54, 0.54, 0.54, 0.55, 0.55, 0.55, 0.56, 0.56, 0.55, 0.56, 0.56, 0.57, 0.57, 0.57, 0.58, 0.58, 0.58, 0.59, 0.59, 0.59, 0.59, 0.6, 0.6, 0.6, 0.6, 0.6, 0.61, 0.61, 0.61, 0.6, 0.61, 0.62, 0.62, 0.62, 0.62, 0.63, 0.62, 0.63, 0.62, 0.63, 0.63, 0.64, 0.63, 0.63, 0.64, 0.64, 0.64, 0.64, 0.65, 0.65, 0.65, 0.65, 0.65, 0.65, 0.66, 0.65, 0.66, 0.65, 0.66, 0.66, 0.66, 0.67, 0.66, 0.67, 0.67, 0.67, 0.67, 0.67, 0.67, 0.67, 0.67, 0.68, 0.68, 0.68, 0.68, 0.68, 0.68, 0.68, 0.68, 0.69, 0.69, 0.68, 0.69, 0.69, 0.69, 0.69, 0.69, 0.69, 0.69, 0.69, 0.69, 0.69, 0.69, 0.7, 0.7, 0.7, 0.7, 0.71, 0.71, 0.71, 0.71, 0.71, 0.71, 0.71, 0.71, 0.71, 0.71, 0.71, 0.71, 0.72, 0.72, 0.72, 0.72, 0.72, 0.72, 0.72, 0.72, 0.73, 0.72, 0.73, 0.73, 0.73, 0.73, 0.73, 0.73, 0.73, 0.73, 0.73, 0.73, 0.73, 0.74, 0.74, 0.73, 0.74, 0.73, 0.73, 0.73, 0.73, 0.73, 0.74, 0.73, 0.73, 0.74, 0.73, 0.74, 0.74, 0.73, 0.74, 0.73, 0.74, 0.74, 0.74, 0.74, 0.74, 0.74, 0.75, 0.75, 0.75, 0.74, 0.75, 0.75, 0.74, 0.74, 0.75, 0.75, 0.75, 0.74, 0.74, 0.75, 0.75, 0.74, 0.75, 0.74, 0.75, 0.75, 0.74, 0.75, 0.74, 0.75, 0.75, 0.75, 0.75, 0.75, 0.75, 0.75, 0.75, 0.75, 0.75, 0.75, 0.75, 0.75, 0.75, 0.75, 0.75, 0.75, 0.75, 0.75, 0.75, 0.76, 0.76, 0.75, 0.76, 0.75, 0.76, 0.75, 0.76, 0.76, 0.76, 0.74, 0.76, 0.76, 0.75, 0.76, 0.76, 0.76, 0.76, 0.76, 0.75, 0.76, 0.76, 0.76, 0.76, 0.76, 0.77, 0.77, 0.77, 0.76, 0.77, 0.77, 0.77, 0.77, 0.77, 0.77, 0.77, 0.77, 0.77, 0.77, 0.78, 0.77, 0.77, 0.77, 0.77, 0.77, 0.77, 0.78, 0.77};
float frontRight[] = {0.42, 0.42, 0.43, 0.43, 0.42, 0.43, 0.43, 0.43, 0.44, 0.44, 0.44, 0.44, 0.44, 0.44, 0.44, 0.45, 0.45, 0.45, 0.45, 0.45, 0.45, 0.46, 0.46, 0.46, 0.46, 0.46, 0.46, 0.47, 0.47, 0.47, 0.47, 0.47, 0.47, 0.47, 0.47, 0.48, 0.48, 0.48, 0.48, 0.48, 0.48, 0.48, 0.48, 0.49, 0.49, 0.49, 0.49, 0.49, 0.49, 0.49, 0.49, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.51, 0.51, 0.51, 0.51, 0.51, 0.51, 0.51, 0.51, 0.51, 0.51, 0.51, 0.52, 0.51, 0.52, 0.52, 0.52, 0.52, 0.53, 0.52, 0.52, 0.53, 0.53, 0.53, 0.52, 0.53, 0.53, 0.53, 0.53, 0.53, 0.53, 0.53, 0.54, 0.54, 0.54, 0.54, 0.54, 0.54, 0.54, 0.54, 0.54, 0.54, 0.54, 0.54, 0.55, 0.55, 0.55, 0.54, 0.55, 0.55, 0.56, 0.55, 0.56, 0.55, 0.56, 0.56, 0.56, 0.55, 0.56, 0.56, 0.56, 0.56, 0.56, 0.56, 0.57, 0.56, 0.56, 0.56, 0.57, 0.58, 0.57, 0.57, 0.57, 0.57, 0.57, 0.57, 0.57, 0.57, 0.57, 0.57, 0.58, 0.57, 0.57, 0.57, 0.58, 0.57, 0.58, 0.58, 0.58, 0.58, 0.58, 0.58, 0.58, 0.58, 0.58, 0.58, 0.58, 0.58, 0.58, 0.58, 0.58, 0.58, 0.59, 0.59, 0.59, 0.59, 0.59, 0.59, 0.59, 0.59, 0.59, 0.59, 0.59, 0.59, 0.59, 0.59, 0.59, 0.59, 0.59, 0.59, 0.59, 0.59, 0.59, 0.59, 0.59, 0.59, 0.59, 0.59, 0.59, 0.59, 0.59, 0.59, 0.59, 0.6, 0.59, 0.6, 0.6, 0.59, 0.59, 0.59, 0.59, 0.6, 0.59, 0.59, 0.58, 0.6, 0.59, 0.59, 0.6, 0.6, 0.59, 0.6, 0.6, 0.6, 0.6, 0.6, 0.6, 0.6, 0.6, 0.6, 0.6, 0.6, 0.6, 0.6, 0.6, 0.6, 0.6, 0.6, 0.6, 0.6, 0.6, 0.6, 0.6, 0.6, 0.6, 0.6, 0.6, 0.61, 0.61, 0.6, 0.61, 0.61, 0.61, 0.61, 0.61, 0.61, 0.61, 0.61, 0.61, 0.61, 0.61, 0.61, 0.61, 0.61, 0.62, 0.61};
float TPS[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9.7, 0, 3.2, 0, 11.2, 0, 0, 0, 2.5, 0, 20.7, 0, 0, 21.2, 21.2, 0, 8, 0, 13.3, 4.1, 3.5, 3.6, 3.3, 0, 7.8, 0, 1.3, 0, 3.1, 3, 3.2, 3.2, 3.4, 3.3, 3.3, 3.3, 3.4, 3.1, 3.3, 3.3, 3, 0, 2.7, 0, 0, 3, 3.1, 3.4, 3.4, 2.4, 0, 0, 0, 3.4, 3.3, 3.2, 3.3, 3.4, 3.2, 0, 0, 3.1, 3.4, 0, 0, 3.2, 3.4, 3.4, 3.1, 3.2, 3.3, 3.4, 3.3, 3.2, 1.2, 3.4, 3.4, 0, 3.3, 3.3, 3.4, 10.6, 0, 0, 0, 23.3, 9.8, 12.7, 34.1, 0, 32.9, 26.5, 36, 36.6, 34.6, 34.6, 38.8, 39.5, 39.5, 32, 0, 11.6, 50.9, 54, 54.7, 48, 47.8, 29.3, 2.8, 20.4, 40.5, 41.2, 43.3, 42, 41.9, 41, 40.4, 5.4, 7.4, 24.9, 21.2, 10.8, 5, 4.9, 0, 45.1, 5.7, 5.8, 5.3, 5.3, 5.3, 17, 27.7, 36.9, 36.5, 36.7, 33.8, 36.9, 40.5, 40.5, 47.7, 44.8, 37.5, 33.2, 33.4, 33.5, 33.7, 16.1, 31.6, 44.8, 45.2, 45.8, 45.6, 42.2, 30.1, 13.6, 13, 9.1, 10.9, 10.2, 11.4, 7, 6.7, 7, 7.4, 18.9, 28.2, 28.6, 19.3, 12.3, 12.1, 13.3, 13.4, 13.2, 13.4, 14.4, 14.4, 19.5, 25.1, 25.8, 24.8, 25, 11.7, 23.6, 23.2, 21.9, 22.7, 32, 33, 42.6, 39.4, 36.6, 36, 36.7, 25.9, 19.7, 18.3, 23.1, 22.4, 28.5, 29.3, 29.3, 21.2, 19, 18.9, 16.2, 9, 6.1, 6.1, 46.5, 0, 5.2, 5.1, 10.1, 11.3, 12.4, 18.7, 23.1, 27.8, 28.6, 28.4, 45.5, 56.6, 58.3, 57.2, 54.2, 55, 65.6, 67.1, 66.4, 67.1, 66.8, 3.6, 36.7, 49.8, 53.8, 55.5, 55.5, 54.9, 54.4, 21.9, 9};
float IPW[] = {3.65, 3.52, 3.48, 3.42, 3.35, 3.29, 5.62, 5.52, 3.2, 3.12, 5.28, 3.01, 3.23, 4.2, 3.06, 3.49, 3.92, 3.87, 3.72, 2.85, 2.99, 3.88, 3.22, 3.98, 3.59, 3.96, 3.81, 3.76, 3.25, 3.71, 1.4, 3.84, 3.21, 2.97, 2.75, 3.04, 2.9, 3.6, 2.82, 3.1, 2.1, 2.21, 3.05, 2.78, 2.77, 2.71, 2.69, 2.95, 2.9, 2.95, 2.85, 2.93, 2.97, 2.96, 2.91, 2.85, 2.9, 2.99, 2.95, 2.98, 3, 2.91, 2.8, 2.93, 2.97, 2.94, 2.97, 3, 3, 2.98, 2.79, 2.87, 2.88, 2.9, 2.98, 2.97, 2.94, 2.87, 2.86, 2.94, 2.87, 2.77, 2.98, 2.86, 2.95, 2.99, 2.96, 2.95, 2.99, 2.89, 2.86, 2.96, 2.93, 2.83, 2.92, 3.74, 4.02, 4.38, 1.4, 4.18, 3.88, 4.29, 1.4, 1.4, 4.22, 4.56, 4.53, 2.81, 4.73, 1.4, 4.47, 4.2, 4.53, 4.62, 4.42, 4.58, 4.83, 4.97, 4.87, 4.8, 2.54, 2.92, 3.21, 4.61, 4.13, 4.14, 4.22, 4.33, 4.34, 4.14, 1.58, 3.08, 3.68, 2.64, 3.16, 2.89, 2.9, 2.89, 4.46, 2.26, 2.9, 2.88, 2.88, 2.78, 3.05, 3.76, 3.93, 3.84, 3.86, 3.8, 3.97, 4.02, 4.05, 4.29, 4.32, 4.14, 3.88, 4.16, 4.1, 4.27, 1.4, 3.37, 4.16, 4.21, 4.26, 4.43, 4.37, 3.53, 3.1, 3.02, 2.99, 2.98, 2.97, 2.91, 2.82, 2.85, 2.84, 2.8, 3.39, 3.71, 3.77, 3.54, 3.23, 3.11, 3.11, 3.12, 3.06, 3.05, 3.08, 3.05, 3.49, 3.69, 3.56, 3.59, 3.61, 2.76, 3.62, 3.56, 3.54, 3.56, 3.98, 3.88, 4.37, 3.4, 4.07, 4.02, 4.09, 3.4, 3.44, 3.41, 3.76, 3.39, 3.8, 3.89, 3.94, 3.54, 3.41, 3.41, 3.38, 2.85, 2.85, 2.77, 3.58, 3.04, 3.04, 2.69, 2.82, 2.82, 3.04, 3.5, 3.62, 3.9, 3.9, 3.92, 4.47, 4.46, 4.2, 4.09, 4.15, 4.26, 4.67, 4.99, 5.17, 5.74, 5.79, 2.6, 3.73, 4.39, 4.55, 4.69, 4.96, 4.99, 4.91, 2.48, 3.24};
float baro[] = {0, 0, 70, 0, 0, 0, 0, 70, 0, 0, 0, 0, 70, 0, 0, 0, 0, 70, 0, 0, 0, 0, 70, 0, 0, 0, 0, 70, 0, 0, 0, 0, 70, 0, 0, 0, 0, 70, 0, 0, 0, 0, 70, 0, 0, 0, 0, 70, 0, 0, 0, 0, 70, 0, 0, 0, 0, 70, 0, 0, 0, 0, 70, 0, 0, 0, 0, 70, 0, 0, 0, 0, 70, 0, 0, 0, 0, 70, 0, 0, 0, 0, 70, 0, 0, 0, 0, 70, 0, 0, 0, 0, 70, 0, 0, 0, 0, 70, 0, 0, 0, 0, 70, 0, 0, 0, 0, 70, 0, 0, 0, 0, 70, 0, 0, 0, 0, 70, 0, 0, 0, 0, 70, 0, 0, 0, 0, 70, 0, 0, 0, 0, 70, 0, 0, 0, 0, 70, 0, 0, 0, 0, 70, 0, 0, 0, 0, 70, 0, 0, 0, 0, 70, 0, 0, 0, 0, 70, 0, 0, 0, 0, 70, 0, 0, 0, 0, 70, 0, 0, 0, 0, 70, 0, 0, 0, 0, 70, 0, 0, 0, 0, 70, 0, 0, 0, 0, 70, 0, 0, 0, 0, 70, 0, 0, 0, 0, 70, 0, 0, 0, 0, 70, 0, 0, 0, 0, 70, 0, 0, 0, 0, 70, 0, 0, 0, 0, 70, 0, 0, 0, 0, 70, 0, 0, 0, 0, 70, 0, 0, 0, 0, 70, 0, 0, 0, 0, 70, 0, 0, 0, 0, 70, 0, 0, 0, 0, 70, 0, 0, 0, 0, 70, 0, 0};
float MAP[] = {0, 0, 20, 20, 11, 24, 0, 0, 11, 0, 15, 37, 79, 60, 72, 64, 78, 66, 63, 72, 60, 66, 76, 83, 86, 86, 92, 93, 93, 86, 80, 81, 70, 70, 68, 68, 65, 67, 82, 62, 77, 78, 70, 69, 68, 67, 67, 67, 67, 67, 67, 67, 67, 67, 52, 67, 67, 60, 67, 64, 67, 67, 67, 67, 67, 67, 67, 67, 67, 67, 67, 67, 67, 67, 67, 62, 67, 67, 60, 63, 67, 67, 67, 67, 59, 67, 67, 66, 67, 67, 67, 68, 59, 66, 66, 81, 85, 88, 87, 84, 84, 85, 75, 81, 89, 93, 92, 67, 93, 80, 87, 79, 93, 75, 85, 94, 92, 88, 80, 74, 61, 41, 58, 79, 85, 77, 71, 68, 66, 63, 43, 45, 66, 64, 56, 49, 48, 51, 94, 70, 62, 52, 53, 52, 67, 83, 89, 88, 87, 85, 87, 87, 87, 88, 86, 77, 68, 67, 67, 65, 61, 42, 73, 78, 76, 74, 71, 65, 50, 49, 49, 49, 51, 54, 58, 61, 62, 64, 71, 83, 84, 82, 76, 72, 71, 72, 72, 72, 72, 72, 75, 79, 82, 83, 83, 78, 73, 81, 81, 80, 85, 87, 90, 90, 87, 86, 86, 82, 71, 68, 69, 71, 74, 78, 79, 78, 70, 69, 68, 61, 60, 61, 66, 61, 60, 56, 58, 70, 75, 78, 86, 86, 89, 90, 95, 97, 96, 95, 94, 93, 95, 92, 89, 87, 85, 45, 57, 83, 83, 82, 81, 78, 77, 65, 46};
float AFR[] = {22.01, 22.01, 22.01, 22.01, 22.01, 22.01, 22.01, 22.01, 22.01, 22.01, 22.01, 22.01, 22.01, 22.01, 22.01, 22.01, 22.01, 22.01, 22.01, 22.01, 22.01, 22.01, 22.01, 22.01, 22.01, 22.01, 22.01, 22.01, 22.01, 22.01, 22.01, 22.01, 22.01, 22.01, 22.01, 22.01, 22.01, 22.01, 22.01, 22.01, 22.01, 22.01, 22.01, 22.01, 22.01, 22.01, 22.01, 22.01, 22.01, 22.01, 22.01, 22.01, 22.01, 22.01, 22.01, 22.01, 22.01, 22.01, 22.01, 22.01, 22.01, 22.01, 22.01, 22.01, 22.01, 22.01, 22.01, 22.01, 22.01, 22.01, 22.01, 22.01, 22.01, 22.01, 22.01, 11.61, 12.88, 12.82, 12.73, 12.88, 12.83, 12.67, 12.66, 12.72, 12.63, 12.38, 12.3, 12.32, 12.33, 12.57, 12.58, 12.61, 12.7, 12.25, 12.2, 13.39, 13.64, 12.22, 12.02, 13.67, 14.02, 12.98, 12.72, 13.64, 13.67, 13.2, 12.32, 13.23, 13.8, 13.79, 16.02, 14.95, 13.02, 12.19, 11.48, 12.17, 12.91, 12.22, 13.48, 13.51, 13.63, 13.79, 14.92, 12.16, 12.8, 13.89, 13.79, 13.23, 12.22, 12.2, 11.97, 12.74, 13.99, 11.6, 11.86, 11.77, 14.85, 16.96, 16.76, 14.27, 15.98, 12.32, 13.72, 15.11, 13.98, 12.22, 12.85, 13.24, 13.38, 13.41, 13.08, 13.2, 12.99, 12.94, 12.51, 12, 12.8, 12.79, 12.02, 11.82, 11.95, 12.01, 12.76, 13.04, 14.05, 14.32, 13.55, 12.1, 11.14, 15.39, 17.4, 17.55, 14.77, 14.46, 12.49, 11.85, 11.95, 11.86, 11.79, 12.52, 13.2, 13.27, 12.55, 11.88, 11.77, 11.77, 11.85, 11.83, 11.82, 11.82, 11.94, 12.27, 12.49, 12.8, 12.82, 12.72, 12.22, 12.1, 12.44, 12.14, 12.04, 12.47, 12.54, 12.86, 12.69, 12.14, 12.04, 11.91, 11.47, 11.32, 11.33, 11.3, 11.41, 11.69, 11.75, 11.72, 11.27, 11.25, 11.25, 11.26, 11.86, 11.86, 12.13, 13.72, 12.94, 12.36, 13.1, 13.7, 12.72, 12.45, 12.33, 12.04, 12.32, 12.13, 11.64, 11.77, 12.02, 12.55, 12.86, 13.08, 12.83, 12.54, 13.04, 14.11, 13.83, 14.32, 14.23, 12.91, 13.54, 14.17, 14.01, 13.33, 13.19, 13.67, 11.64};
float IAT[] = {25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 28, 28, 28, 28, 28, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 28, 28, 28, 28, 28, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29};
float engineTemp[] = {0, 0, 44, 0, 0, 0, 0, 43, 0, 0, 0, 0, 42, 0, 0, 0, 0, 42, 0, 0, 0, 0, 41, 0, 0, 0, 0, 41, 0, 0, 0, 0, 40, 0, 0, 0, 0, 40, 0, 0, 0, 0, 40, 0, 0, 0, 0, 40, 0, 0, 0, 0, 40, 0, 0, 0, 0, 40, 0, 0, 0, 0, 40, 0, 0, 0, 0, 40, 0, 0, 0, 0, 40, 0, 0, 0, 0, 40, 0, 0, 0, 0, 40, 0, 0, 0, 0, 41, 0, 0, 0, 0, 41, 0, 0, 0, 0, 41, 0, 0, 0, 0, 42, 0, 0, 0, 0, 42, 0, 0, 0, 0, 42, 0, 0, 0, 0, 43, 0, 0, 0, 0, 43, 0, 0, 0, 0, 43, 0, 0, 0, 0, 44, 0, 0, 0, 0, 44, 0, 0, 0, 0, 45, 0, 0, 0, 0, 45, 0, 0, 0, 0, 45, 0, 0, 0, 0, 46, 0, 0, 0, 0, 46, 0, 0, 0, 0, 46, 0, 0, 0, 0, 46, 0, 0, 0, 0, 47, 0, 0, 0, 0, 47, 0, 0, 0, 0, 47, 0, 0, 0, 0, 48, 0, 0, 0, 0, 48, 0, 0, 0, 0, 48, 0, 0, 0, 0, 49, 0, 0, 0, 0, 49, 0, 0, 0, 0, 49, 0, 0, 0, 0, 50, 0, 0, 0, 0, 50, 0, 0, 0, 0, 50, 0, 0, 0, 0, 50, 0, 0, 0, 0, 51, 0, 0, 0, 0, 51, 0, 0, 0, 0, 51, 0, 0};
float oilPressure[] = {-14.83, -14, -14.08, -8.93, 0.8, -9.55, -11.43, -9.7, -17.68, -13.68, -17.05, -3.43, 53.6, 15.72, 53.27, 49.77, 51.58, 35.95, 54.23, 44.35, 33.33, 37.33, 48.6, 55.52, 53.35, 51.7, 42.15, 42.98, 45, 45.92, 42.63, 51.92, 23.83, 52.4, 49.9, 50.8, 47.5, 39.73, 48.95, 51.3, 58.67, 53.5, 52.88, 51.42, 51.92, 50.52, 52.42, 51.73, 51.98, 56.33, 51.63, 51.65, 51.33, 52.92, 51, 52.4, 46.2, 51.7, 51.42, 51.98, 52.1, 50.9, 61.88, 51.85, 52.48, 51.27, 51.13, 37.78, 52.45, 52.9, 51.7, 53.85, 48.8, 45.85, 52.17, 56.45, 35.1, 52.08, 52.67, 62.6, 51.77, 52.75, 52.23, 52.1, 57.58, 51.9, 51.85, 52.05, 51.77, 39.05, 52.15, 53.08, 57.6, 51.15, 61.38, 54.52, 50.98, 54.23, 57.95, 57.85, 57.8, 57.55, 60.77, 57.8, 55.3, 45.6, 54.05, 52.15, 40.65, 56, 54.33, 53.85, 55.58, 63.63, 63.65, 57.75, 58.13, 60.3, 61.7, 62.42, 63.23, 61.23, 57.83, 60.17, 59.25, 60.23, 60.05, 61.75, 62.42, 61.75, 61.67, 59, 59.2, 59.42, 59.73, 58.58, 58.7, 57.23, 54.83, 56.42, 57.77, 58, 57.02, 57.58, 56.02, 55.92, 56.05, 57.4, 57.5, 57.83, 57.7, 58.45, 58.65, 58.85, 59.05, 59.98, 60.77, 60.67, 62.52, 62.25, 62.5, 60.75, 60.17, 60.83, 59.98, 60.95, 61.67, 61.6, 60.52, 61.33, 60.92, 60.77, 59.25, 58.48, 58.55, 57.25, 29.08, 21.55, 54.75, 40.55, 56.92, 57.55, 54.17, 57.38, 57.3, 57.4, 56.92, 22.58, 15.35, 14.65, 14.83, 10.97, 8.78, 7.53, 13.58, 14.33, 57.27, 56.92, 57.83, 57.45, 57.3, 57.4, 58.67, 58.27, 59.2, 59.02, 59.38, 59.3, 59.13, 58.55, 59.67, 58.75, 57.02, 27.5, 26.38, 26.58, 67.55, 59.52, 56, 57.4, 58.15, 57.63, 54.88, 44.42, 57.85, 57.63, 56.45, 55.75, 55.42, 55.02, 55.65, 55.58, 56.65, 55.15, 56.02, 56.67, 57.9, 58.33, 58.5, 59.1, 59.02, 60.13, 61.58, 61.75, 62.5, 62.6, 59.58, 60.02, 60.45, 61.3, 62.05, 62.7, 62.5, 63.5, 62.15};
float oilTemp[] = {0, 0, 44, 0, 0, 0, 0, 45, 0, 0, 0, 0, 45, 0, 0, 0, 0, 46, 0, 0, 0, 0, 46, 0, 0, 0, 0, 46, 0, 0, 0, 0, 47, 0, 0, 0, 0, 47, 0, 0, 0, 0, 47, 0, 0, 0, 0, 47, 0, 0, 0, 0, 47, 0, 0, 0, 0, 47, 0, 0, 0, 0, 47, 0, 0, 0, 0, 47, 0, 0, 0, 0, 47, 0, 0, 0, 0, 47, 0, 0, 0, 0, 47, 0, 0, 0, 0, 47, 0, 0, 0, 0, 47, 0, 0, 0, 0, 47, 0, 0, 0, 0, 47, 0, 0, 0, 0, 47, 0, 0, 0, 0, 48, 0, 0, 0, 0, 48, 0, 0, 0, 0, 48, 0, 0, 0, 0, 48, 0, 0, 0, 0, 48, 0, 0, 0, 0, 48, 0, 0, 0, 0, 48, 0, 0, 0, 0, 48, 0, 0, 0, 0, 48, 0, 0, 0, 0, 49, 0, 0, 0, 0, 49, 0, 0, 0, 0, 49, 0, 0, 0, 0, 49, 0, 0, 0, 0, 49, 0, 0, 0, 0, 50, 0, 0, 0, 0, 50, 0, 0, 0, 0, 50, 0, 0, 0, 0, 50, 0, 0, 0, 0, 50, 0, 0, 0, 0, 51, 0, 0, 0, 0, 51, 0, 0, 0, 0, 51, 0, 0, 0, 0, 52, 0, 0, 0, 0, 52, 0, 0, 0, 0, 52, 0, 0, 0, 0, 52, 0, 0, 0, 0, 53, 0, 0, 0, 0, 53, 0, 0, 0, 0, 53, 0, 0};
float fuelTemp[] = {0, 0, 22, 0, 0, 0, 0, 22, 0, 0, 0, 0, 22, 0, 0, 0, 0, 22, 0, 0, 0, 0, 22, 0, 0, 0, 0, 22, 0, 0, 0, 0, 22, 0, 0, 0, 0, 22, 0, 0, 0, 0, 22, 0, 0, 0, 0, 22, 0, 0, 0, 0, 22, 0, 0, 0, 0, 22, 0, 0, 0, 0, 22, 0, 0, 0, 0, 22, 0, 0, 0, 0, 22, 0, 0, 0, 0, 22, 0, 0, 0, 0, 22, 0, 0, 0, 0, 22, 0, 0, 0, 0, 22, 0, 0, 0, 0, 22, 0, 0, 0, 0, 22, 0, 0, 0, 0, 22, 0, 0, 0, 0, 22, 0, 0, 0, 0, 22, 0, 0, 0, 0, 22, 0, 0, 0, 0, 22, 0, 0, 0, 0, 22, 0, 0, 0, 0, 22, 0, 0, 0, 0, 22, 0, 0, 0, 0, 22, 0, 0, 0, 0, 22, 0, 0, 0, 0, 22, 0, 0, 0, 0, 22, 0, 0, 0, 0, 22, 0, 0, 0, 0, 22, 0, 0, 0, 0, 22, 0, 0, 0, 0, 22, 0, 0, 0, 0, 22, 0, 0, 0, 0, 22, 0, 0, 0, 0, 22, 0, 0, 0, 0, 22, 0, 0, 0, 0, 22, 0, 0, 0, 0, 22, 0, 0, 0, 0, 22, 0, 0, 0, 0, 22, 0, 0, 0, 0, 22, 0, 0, 0, 0, 22, 0, 0, 0, 0, 22, 0, 0, 0, 0, 22, 0, 0, 0, 0, 22, 0, 0, 0, 0, 22, 0, 0};
float xAccel[] = {-0.01, 0.02, 0.01, 0.08, 0.07, 0.08, 0.06, -0.01, 0.04, 0.04, 0.05, 0.01, 0.09, 0.03, 0.01, 0.06, 0, -0.04, -0.05, -0.15, -0.05, 0.06, 0.05, 0.08, 0, -0.03, 0.01, 0.04, 0.12, 0.08, 0.08, 0.06, 0.12, 0.16, 0.07, 0.05, 0.05, 0.01, 0.08, 0.09, 0.08, 0.02, -0.02, 0.06, 0.06, 0.06, 0.03, 0.01, 0.05, 0.06, 0.05, 0.07, 0.07, 0.07, 0.05, 0.05, 0.07, 0.02, 0, 0.02, 0, 0.03, 0.05, 0.05, 0, -0.02, 0.04, 0.06, 0.01, 0, 0.03, 0.04, 0.01, -0.02, -0.01, 0.05, -0.01, 0.03, 0.04, 0.02, 0.05, 0.03, -0.02, 0.03, 0.03, 0.01, 0.01, 0, 0, 0, 0.01, 0.03, 0.06, 0.04, 0.07, 0.07, 0.05, -0.06, 0.06, -0.01, 0.16, 0.06, -0.02, 0.06, 0.08, 0.02, 0.1, -0.01, -0.08, 0.07, 0.15, 0.04, 0.03, 0.19, 0.14, 0.21, 0.15, 0.21, 0.31, 0.3, 0.26, 0.71, 0.33, 0.23, 0.32, 0.17, -0.37, -0.05, -0.08, -0.01, -0.07, 0.09, -0.07, -0.16, -0.2, -0.07, 0.11, 0.16, 0.02, -0.02, 0.1, 0.25, 0.28, 0.3, 0.35, 0.48, 0.69, 0.58, 0.58, 0.66, 0.66, 0.42, 0.36, 0.49, 0.49, 0.48, 0.28, -0.26, -0.35, -0.53, -0.47, -0.56, -0.45, -0.57, -0.44, -0.15, -0.22, -0.47, -0.3, 0.31, 0.01, -0.15, -0.3, -0.59, -0.65, -0.78, -0.81, -0.87, -0.89, -0.75, -0.66, -0.52, -0.48, -0.52, -0.61, -0.8, -0.91, -0.97, -1.05, -1.15, -1.19, -1.1, -1, -0.94, -0.84, -0.42, 0.15, 0.51, 0.88, 0.86, 0.91, 0.88, 0.73, 0.75, 0.66, 0.61, 0.32, -0.11, -0.38, -0.31, -0.61, -0.76, -0.85, -0.99, -0.93, -0.81, -0.4, 0.21, 0.52, 0.5, 0.58, 0.74, 0.84, 0.74, 0.6, 0.54, 0.68, 0.63, 0.28, -0.43, -0.64, -0.66, -0.82, -0.79, -0.63, -0.56, -0.67, -0.76, -0.8, -0.64, -0.57, -0.57, -0.57, -0.2, -0.19, 0, -0.11, -0.03, -0.13, -0.08, 0.03, 0.1, 0.14, 0.1, 0.2};
float yAccel[] = {-0.06, -0.05, -0.08, -0.06, -0.06, -0.02, -0.05, -0.12, -0.09, -0.06, -0.05, -0.07, -0.1, -0.1, -0.11, -0.06, -0.09, -0.05, -0.09, -0.15, -0.07, -0.02, 0.03, 0.22, 0.21, 0.2, 0.21, 0.1, -0.13, 0.07, -0.04, -0.02, -0.06, -0.06, -0.08, -0.08, -0.2, -0.1, 0.03, -0.06, -0.05, -0.03, -0.1, -0.08, -0.09, -0.08, -0.06, -0.07, -0.12, -0.13, -0.13, -0.13, -0.12, -0.11, -0.14, -0.13, -0.14, -0.14, -0.13, -0.17, -0.19, -0.13, -0.07, -0.08, -0.07, -0.05, -0.06, -0.07, -0.07, -0.05, -0.03, -0.05, -0.07, -0.06, -0.06, -0.06, -0.06, -0.05, -0.06, -0.05, -0.04, -0.07, -0.08, -0.05, -0.05, -0.07, -0.06, -0.05, -0.1, -0.08, -0.07, -0.03, -0.05, -0.07, -0.05, -0.05, -0.04, -0.08, -0.07, -0.01, 0.09, 0.16, 0.15, 0.18, 0.19, 0.21, 0.19, 0.19, -0.04, 0.14, 0.14, 0.07, 0.04, 0.03, 0.09, 0.2, 0.24, 0.21, 0.43, 0.43, 0.61, 0.11, -0.16, -0.04, 0.12, 0.2, 0.37, 0.22, 0.2, 0.2, -0.06, -0.1, -0.28, 0.03, -0.11, -0.16, -0.25, -0.19, -0.2, -0.27, -0.39, -0.44, -0.39, -0.34, -0.32, -0.19, 0.09, 0.11, 0.09, 0.1, 0.13, -0.03, 0.09, 0.15, 0.23, 0.27, 0.55, 0.45, 0.07, 0.15, 0.1, 0.23, 0.03, 0.13, 0.12, 0.08, 0.18, 0.21, -0.14, -0.53, -0.43, -0.44, -0.46, -0.46, -0.32, -0.31, -0.14, -0.19, -0.17, -0.07, -0.02, 0, 0.01, -0.09, -0.12, -0.18, -0.15, -0.16, -0.15, -0.19, -0.14, -0.07, -0.01, 0.02, 0.04, 0.02, -0.07, -0.02, -0.01, -0.05, -0.04, 0.01, 0.01, 0.07, -0.01, 0.06, 0.03, 0.08, -0.08, -0.1, -0.06, -0.14, -0.12, -0.05, -0.02, 0, -0.11, -0.04, -0.04, -0.24, -0.33, -0.42, -0.43, -0.42, -0.48, -0.46, -0.36, -0.25, 0.01, -0.16, -0.19, 0.01, 0.14, -0.07, 0.08, 0.09, 0.17, 0.22, 0.1, 0.27, 0.16, 0.35, 0.58, 0.48, 0.56, 0.14, 0.05, 0.18, 0.31, 0.29, 0.3, 0.24, 0.17, -0.08, -0.26};
float zAccel[] = {-1, -1.02, -0.99, -0.99, -0.98, -1.02, -1.06, -0.95, -1, -1.05, -1.05, -1.11, -1.05, -0.99, -0.96, -0.95, -1.03, -1.06, -1.04, -1.26, -1.11, -0.82, -0.93, -1.09, -0.99, -1.08, -1.03, -0.95, -0.85, -1.08, -0.97, -1.08, -0.97, -0.97, -1.06, -1, -0.91, -1.01, -1.08, -1.04, -0.99, -1.09, -0.96, -0.96, -1, -0.99, -1.02, -0.99, -1, -1.04, -1.03, -1.01, -0.99, -0.98, -1.01, -1.01, -1.01, -1, -1.02, -0.99, -1.04, -1.02, -0.95, -0.99, -1.04, -1.06, -1.04, -1.06, -1.05, -1.02, -0.98, -1.02, -1.07, -1.06, -1.02, -1.01, -1.07, -1.05, -1.06, -1, -1.03, -1.02, -0.99, -1.01, -1.05, -0.96, -1.03, -1.04, -1.03, -1.03, -1.03, -0.99, -1, -0.98, -1.04, -1.05, -0.81, -0.87, -0.96, -1, -0.98, -0.93, -0.84, -1.04, -0.68, -1.33, -1.08, -1.07, -0.82, -1.07, -1.02, -1.14, -1.41, -1.08, -0.69, -0.88, -0.94, -0.92, -0.96, -0.88, -0.98, -0.96, -1.04, -0.98, -1.16, -0.8, -0.63, -0.8, -0.69, -0.98, -0.7, -1.07, -1.05, -1.19, -1.15, -0.99, -0.91, -0.93, -0.9, -1.07, -1.18, -1.33, -1.11, -1.16, -1.06, -1.08, -1.22, -0.84, -0.93, -0.99, -0.94, -0.7, -0.33, -0.62, -0.83, -1.3, -1.41, -1.25, -0.54, -0.7, -0.49, -0.89, -0.92, -1.07, -1.13, -1.05, -1.23, -0.91, -0.61, -1.12, -0.89, -0.98, -0.6, -1.08, -1.05, -1, -1.05, -1, -1.38, -0.7, -0.7, -0.74, -0.84, -0.66, -0.88, -0.86, -0.91, -0.53, -0.68, -0.25, -0.79, -1.3, -1.15, -1.09, -0.93, -0.82, -0.94, -0.68, -0.81, -0.86, -0.89, -0.65, -0.92, -1.1, -1.12, -1.09, -1.07, -1.04, -1.13, -1.16, -1.06, -0.95, -0.92, -0.96, -0.93, -0.96, -1.11, -0.95, -1.06, -0.75, -1.25, -0.96, -0.91, -0.91, -0.91, -0.93, -0.88, -0.96, -1.36, -1.49, -1.33, -1.15, -1.09, -0.98, -0.91, -0.87, -0.94, -1.02, -1.06, -0.46, -0.79, -1.07, -1.1, -1.06, -1.07, -0.77, -0.91, -0.97, -0.88, -0.88, -1.07, -0.95, -0.64, -0.51, -0.66};
float roll[] = {-1, 0, 0, 0, 0, 0, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 1, -1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, -1, 0, 0, 0, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, 0, 0, 0, 0, -1, 0, -1, 0, 0, 0, 0, 0, -1, 0, 0, 0, 0, -1, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, 0, 0, 0, -1, 0, -1, 0, 0, 0, 0, 0, 1, 0, 0, 0, -3, -3, -2, 1, 0, -1, 3, -1, 0, 1, 2, 0, -2, -1, -2, -1, -1, 2, 0, -1, -1, 0, 0, 3, 1, 0, 0, 0, 1, 2, 0, 0, 3, -1, 1, 1, 2, 2, 0, -1, -1, 3, 8, 7, 4, 2, -1, 0, -1, -2, -2, -1, 2, 1, -4, 1, -1, -1, -2, -1, 1, 0, -2, -3, -1, -2, -2, -1, -1, -1, -1, 0, 0, -4, -4, -5, -1, -1, -2, -2, -3, -5, -6, -2, -1, 1, 1, 3, 2, 3, 2, 2, 2, 3, 2, -1, 0, 0, -2, -2, -3, -3, -5, -5, 0, -3, 3, 2, 0, 3, 1, 1, 2, 3, 6, 3, 0, -1, -2, -4, -3, -4, -1, -1, -3, -2, -2, -1, 0, -2, -1, -1, 0, -1, -2, -1, -1, -1, 0, 2, 1};
float pitch[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 1, -1, 0, 0, 0, 0, 0, 0, -1, 1, 1, 0, 2, 1, 3, 3, -3, 0, -2, -1, -2, 0, -2, 1, 2, 0, -2, -1, 0, 1, 0, -2, 1, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 2, 2, 0, 0, 1, 2, 1, 1, 0, -3, -4, 0, -2, 0, -1, 3, 0, 1, 4, 3, -1, -4, 0, 1, 3, -1, -1, -1, 1, -1, -1, -2, -1, 1, -1, -1, -2, 0, -2, -2, -1, -1, -1, -2, -3, -3, -2, -1, -1, 0, 0, 0, 2, 0, 1, -3, 0, -2, -1, 0, 0, -1, 2, -1, -1, 0, -1, -1, -2, -1, 0, -1, 1, -1, 0, 0, 0, 1, 0, 1, 0, -2, -3, -2, -1, -2, -2, 1, 1, 0, -1, -1, -1, 1, 0, 1, -1, -2, -2, -3, 1, 0, -1, -2, 1, 1, -1, -1};
float yaw[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 4, 6, 9, 11, 11, 12, 12, 12, 12, 12, 12, 11, 9, 6, 6, 5, 5, 4, 4, 4, 3, 3, 3, 3, 3, 2, 2, 2, 2, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 2, 4, 5, 5, 7, 10, 13, 14, 14, 16, 15, 15, 16, 14, 12, 12, 11, 10, 8, 3, -1, -2, 0, 2, 5, 3, 0, -3, -7, -6, 0, 1, 2, 3, 5, 9, 14, 16, 21, 31, 35, 38, 38, 37, 33, 27, 27, 26, 25, 23, 15, 0, -19, -26, -28, -24, -23, -22, -21, -13, -10, -11, -5, -4, -6, -9, -13, -17, -26, -39, -42, -43, -45, -39, -34, -30, -30, -30, -32, -37, -47, -45, -51, -56, -60, -59, -55, -51, -43, -26, 2, 23, 39, 36, 42, 37, 32, 33, 32, 26, 12, -4, -17, -17, -24, -33, -41, -46, -43, -35, -20, 4, 19, 29, 29, 34, 41, 40, 35, 37, 40, 40, 21, -13, -38, -51, -56, -56, -54, -45, -46, -46, -44, -39, -34, -29, -24, -18, -8, -2, 0, 0, -1, -2, -2, 1, 0, 1, 2};
double longitude[] = {-96.767212, -96.76722, -96.767273, -96.767311, -96.767326, -96.767334, -96.767334, -96.767342, -96.767349, -96.767357, -96.767364, -96.767364, -96.767364, -96.767364, -96.767364, -96.767364, -96.767364, -96.767364, -96.767364, -96.767364, -96.767364, -96.767364, -96.767364, -96.767364, -96.767364, -96.767372, -96.76738, -96.767387, -96.767395, -96.767403, -96.76741, -96.767418, -96.767426, -96.767433, -96.767441, -96.767441, -96.767448, -96.767456, -96.767464, -96.767471, -96.767479, -96.767479, -96.767487, -96.767494, -96.767502, -96.767502, -96.767509, -96.767532, -96.767532, -96.76754, -96.76754, -96.767548, -96.767548, -96.767555, -96.767555, -96.767555, -96.767555, -96.767563, -96.767563, -96.767563, -96.767563, -96.767563, -96.767563, -96.767563, -96.767563, -96.767563, -96.767563, -96.767563, -96.767563, -96.767563, -96.767563, -96.767563, -96.767563, -96.767563, -96.767563, -96.767563, -96.767563, -96.767563, -96.767563, -96.767563, -96.767563, -96.767563, -96.767563, -96.767563, -96.767563, -96.767563, -96.767563, -96.767563, -96.767563, -96.767563, -96.767563, -96.767563, -96.767563, -96.767563, -96.767563, -96.767563, -96.767563, -96.767563, -96.767563, -96.767563, -96.767563, -96.767563, -96.767563, -96.767563, -96.767563, -96.76757, -96.767578, -96.767578, -96.767593, -96.767601, -96.767601, -96.767609, -96.767616, -96.767624, -96.767632, -96.767639, -96.767639, -96.767647, -96.767654, -96.767662, -96.76767, -96.767677, -96.767677, -96.767685, -96.767685, -96.767685, -96.767685, -96.767685, -96.767685, -96.767685, -96.767685, -96.767693, -96.767693, -96.767693, -96.767693, -96.767693, -96.7677, -96.7677, -96.767708, -96.767708, -96.767708, -96.767708, -96.767708, -96.767708, -96.767708, -96.7677, -96.767693, -96.767685, -96.767677, -96.767662, -96.767639, -96.767624, -96.767601, -96.767586, -96.767563, -96.767532, -96.767509, -96.767479, -96.767448, -96.767418, -96.767395, -96.767364, -96.767334, -96.767311, -96.767281, -96.76725, -96.767227, -96.767204, -96.767174, -96.767151, -96.767128, -96.767105, -96.767082, -96.767059, -96.767044, -96.767036, -96.767029, -96.767029, -96.767029, -96.767029, -96.767044, -96.767044, -96.767059, -96.767082, -96.76709, -96.767113, -96.767136, -96.767158, -96.767181, -96.767204, -96.767235, -96.76725, -96.767281, -96.767296, -96.767311, -96.767326, -96.767334, -96.767349, -96.767364, -96.767387, -96.767403, -96.767433, -96.767456, -96.767487, -96.767517, -96.767548, -96.767578, -96.767601, -96.767639, -96.76767, -96.7677, -96.767731, -96.767761, -96.767784, -96.767807, -96.76783, -96.767845, -96.767868, -96.767883, -96.767906, -96.767929, -96.76796, -96.767975, -96.767998, -96.768028, -96.768044, -96.768066, -96.768082, -96.768105, -96.76812, -96.768135, -96.76815, -96.768166, -96.768181, -96.768196, -96.768211, -96.768227, -96.768234, -96.768242, -96.76825, -96.768257, -96.768257, -96.768257, -96.76825, -96.76825, -96.768242, -96.768234, -96.768234, -96.768234, -96.768227, -96.768219, -96.768211, -96.768204, -96.768196, -96.768196};
double latitude[] = {40.847313, 40.847263, 40.847012, 40.846783, 40.846725, 40.846684, 40.846684, 40.846664, 40.846626, 40.8466, 40.846577, 40.846565, 40.846558, 40.846558, 40.846558, 40.846558, 40.846558, 40.846558, 40.846558, 40.846558, 40.846558, 40.846558, 40.846558, 40.846554, 40.846554, 40.84655, 40.846546, 40.846539, 40.846531, 40.846527, 40.846523, 40.846516, 40.846512, 40.846504, 40.846504, 40.846497, 40.846493, 40.846485, 40.846485, 40.846458, 40.846443, 40.846436, 40.846432, 40.846428, 40.846424, 40.84642, 40.846416, 40.846405, 40.846401, 40.846401, 40.846397, 40.846394, 40.846394, 40.84639, 40.84639, 40.846386, 40.846386, 40.846386, 40.846386, 40.846386, 40.846382, 40.846382, 40.846382, 40.846382, 40.846382, 40.846382, 40.846382, 40.846382, 40.846382, 40.846382, 40.846382, 40.846382, 40.846382, 40.846382, 40.846382, 40.846382, 40.846382, 40.846382, 40.846382, 40.846382, 40.846382, 40.846382, 40.846382, 40.846382, 40.846382, 40.846382, 40.846382, 40.846382, 40.846382, 40.846382, 40.846382, 40.846382, 40.846382, 40.846382, 40.846382, 40.846382, 40.846382, 40.846382, 40.846382, 40.846382, 40.846382, 40.846386, 40.846386, 40.846382, 40.846382, 40.846378, 40.846374, 40.846371, 40.846367, 40.846359, 40.846355, 40.846348, 40.84634, 40.846336, 40.846329, 40.846317, 40.846302, 40.846287, 40.846272, 40.846268, 40.846249, 40.846222, 40.846203, 40.846184, 40.846165, 40.846146, 40.846123, 40.846104, 40.846077, 40.846054, 40.846027, 40.846004, 40.845978, 40.845955, 40.845928, 40.845921, 40.845882, 40.845863, 40.84584, 40.845818, 40.845798, 40.845779, 40.845764, 40.845749, 40.845734, 40.845722, 40.845711, 40.845696, 40.845684, 40.845673, 40.845661, 40.845654, 40.845646, 40.845634, 40.845631, 40.845627, 40.845623, 40.845623, 40.845619, 40.845612, 40.845604, 40.845592, 40.845581, 40.84557, 40.845554, 40.845531, 40.845512, 40.845493, 40.845474, 40.845451, 40.845428, 40.845409, 40.84539, 40.845371, 40.845352, 40.845333, 40.845314, 40.845295, 40.845276, 40.845253, 40.845234, 40.845215, 40.8452, 40.845184, 40.845169, 40.845154, 40.845142, 40.845135, 40.845131, 40.845131, 40.845131, 40.845135, 40.845146, 40.845158, 40.845177, 40.845192, 40.845211, 40.84523, 40.845245, 40.845257, 40.845272, 40.845284, 40.845291, 40.845295, 40.845299, 40.845299, 40.845299, 40.845299, 40.845295, 40.845299, 40.845299, 40.845306, 40.845314, 40.845322, 40.845333, 40.845352, 40.845371, 40.84539, 40.845409, 40.845425, 40.84544, 40.845451, 40.845459, 40.845467, 40.84547, 40.84547, 40.84547, 40.845467, 40.845459, 40.845455, 40.845455, 40.845451, 40.845455, 40.845455, 40.845463, 40.845474, 40.845482, 40.845493, 40.845512, 40.845528, 40.845543, 40.845562, 40.845585, 40.845604, 40.845627, 40.845654, 40.845676, 40.845703, 40.84573, 40.845753, 40.845783, 40.84581, 40.84584, 40.845871, 40.845905};
float speed[] = {0, 0, 80.56, 59.52, 43.79, 33.96, 27.51, 22.51, 18.82, 14.59, 11.39, 9.21, 7.48, 6.42, 5.31, 4.42, 3.8, 0, 0, 0, 0, 0, 0, 0, 2.22, 3.81, 4.92, 5.72, 6.19, 6.19, 6.19, 5.96, 6, 5.99, 5.87, 5.88, 5.81, 5.58, 5.56, 5.61, 5.55, 5.35, 5.4, 5.38, 5.05, 5.16, 5.08, 5.18, 4.96, 4.72, 4.27, 3.88, 3.72, 3.22, 3.14, 2.61, 2.27, 2.01, 1.54, 1.31, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1.11, 2.11, 3.08, 3.95, 5.16, 6.13, 7.3, 8.72, 9.62, 10.1, 11.28, 12.11, 12.39, 12.93, 14, 15.73, 17.09, 19.31, 20.07, 22.15, 23.63, 23.67, 22.96, 24.13, 25.31, 26.74, 28.09, 29.34, 30.23, 31.23, 31.21, 30.62, 30.15, 30.37, 30.11, 28.73, 27.39, 26.63, 25.75, 24.41, 22.25, 20.67, 19.33, 18.2, 17.87, 17.93, 18.98, 19.83, 20.65, 20.99, 21.65, 22.26, 23, 24.09, 25.05, 26.75, 27.94, 28.82, 29.42, 30.14, 30.9, 30.56, 31.64, 32.79, 34.21, 35.39, 36.41, 36.6, 35.49, 34.32, 32.77, 31.07, 28.92, 27.04, 25.49, 24.82, 24.3, 23.86, 23.84, 24.46, 24.92, 24.81, 24.92, 24.7, 24.57, 24.44, 24.28, 24.1, 24.1, 23.47, 23.59, 23.66, 23.92, 24.51, 24.83, 25.3, 25.49, 25.69, 25.67, 26.21, 26.31, 26.75, 27.09, 27.78, 28.22, 28.74, 29.19, 29.54, 29.34, 29, 28.65, 28.54, 28.18, 28.49, 28.74, 28.96, 28.98, 28.9, 28.67, 27.35, 25.71, 24.15, 22.79, 21.48, 19.73, 18.29, 17.3, 16.91, 16.33, 16.02, 16.09, 15.88, 16.49, 16.51, 17.27, 18.21, 19.35, 20.44, 21.36, 22.23, 23.8, 25.83, 28.33, 30.2, 31.56, 31.43, 31.04, 32.64, 34.27, 35.85, 37.34, 38.61, 40.11, 40.47};
float distance[] = {0, 0, 0.003, 0.007, 0.01, 0.012, 0.014, 0.015, 0.016, 0.017, 0.018, 0.018, 0.019, 0.019, 0.02, 0.02, 0.02, 0.02, 0.02, 0.02, 0.02, 0.02, 0.02, 0.02, 0.02, 0.02, 0.021, 0.021, 0.021, 0.022, 0.022, 0.022, 0.023, 0.023, 0.023, 0.024, 0.024, 0.024, 0.025, 0.025, 0.025, 0.025, 0.026, 0.026, 0.026, 0.027, 0.027, 0.027, 0.027, 0.028, 0.028, 0.028, 0.028, 0.029, 0.029, 0.029, 0.029, 0.029, 0.029, 0.029, 0.029, 0.029, 0.029, 0.029, 0.029, 0.029, 0.029, 0.029, 0.029, 0.029, 0.029, 0.029, 0.029, 0.029, 0.029, 0.029, 0.029, 0.029, 0.029, 0.029, 0.029, 0.029, 0.029, 0.029, 0.029, 0.029, 0.029, 0.029, 0.029, 0.029, 0.029, 0.029, 0.029, 0.029, 0.029, 0.029, 0.029, 0.029, 0.029, 0.029, 0.029, 0.029, 0.029, 0.03, 0.03, 0.03, 0.03, 0.031, 0.031, 0.032, 0.032, 0.033, 0.034, 0.034, 0.035, 0.036, 0.037, 0.038, 0.039, 0.039, 0.04, 0.042, 0.044, 0.045, 0.046, 0.048, 0.049, 0.051, 0.052, 0.054, 0.056, 0.057, 0.059, 0.061, 0.062, 0.063, 0.066, 0.067, 0.069, 0.07, 0.072, 0.073, 0.074, 0.075, 0.076, 0.077, 0.078, 0.079, 0.08, 0.081, 0.083, 0.084, 0.085, 0.086, 0.088, 0.089, 0.09, 0.092, 0.093, 0.095, 0.097, 0.098, 0.1, 0.102, 0.104, 0.106, 0.107, 0.109, 0.112, 0.114, 0.115, 0.117, 0.119, 0.121, 0.122, 0.124, 0.125, 0.127, 0.128, 0.129, 0.131, 0.132, 0.133, 0.135, 0.136, 0.137, 0.139, 0.14, 0.141, 0.143, 0.144, 0.145, 0.147, 0.148, 0.149, 0.151, 0.152, 0.154, 0.155, 0.156, 0.158, 0.159, 0.161, 0.162, 0.164, 0.165, 0.167, 0.169, 0.17, 0.172, 0.173, 0.175, 0.177, 0.178, 0.18, 0.181, 0.183, 0.185, 0.186, 0.188, 0.189, 0.191, 0.192, 0.194, 0.195, 0.196, 0.197, 0.198, 0.199, 0.2, 0.201, 0.202, 0.202, 0.203, 0.204, 0.205, 0.206, 0.207, 0.208, 0.21, 0.211, 0.212, 0.213, 0.215, 0.217, 0.218, 0.22, 0.222, 0.223, 0.225, 0.227, 0.229, 0.231, 0.234, 0.236};

struct Data
{
    unsigned int count;
    float rearLeft;
    float rearRight;
    float frontLeft;
    float frontRight;
    float TPS;
    float IPW;
    float baro;
    float MAP;
    float AFR;
    float IAT;
    float engineTemp;
    float oilPressure;
    float oilTemp;
    float fuelTemp;
    float xAccel;
    float yAccel;
    float zAccel;
    float roll;
    float pitch;
    float yaw;
    double longitude;
    double latitiude;
    float speed;
    float distance;
};

void sendData(struct Data val, int sock)
{
    send(sock, (struct Data *)(&val), sizeof(val), 0);
}

int main(int argc, char const *argv[])
{
    int sock = 0, valread;
    struct sockaddr_in serv_addr;

    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }

    memset(&serv_addr, 0, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    struct hostent *server = gethostbyname("127.0.0.1");
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    //serv_addr.sin_addr.s_addr = htonl("127.0.0.1");

    // if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
    //     printf("\nInvalid address/ Address not supported \n");
    //     return -1;
    // }

    // if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
    //     printf("\nConnection Failed \n");
    //     return -1;
    // }

    time_t startTime = clock();
    time_t check = 0;
    int counter = 0;
    while (counter < 255)
    {
        check = (float)(clock() - startTime) / CLOCKS_PER_SEC * 1000;
        while (check >= 100)
        { //Send data every 100ms
            struct Data val;
            //Read from CAN bus here
            val.count = counter;
            val.rearLeft = rearLeft[counter];
            val.rearRight = rearRight[counter];
            val.frontLeft = frontLeft[counter];
            val.frontRight = frontRight[counter];
            val.TPS = TPS[counter];
            val.IPW = IPW[counter];
            val.baro = baro[counter];
            val.MAP = MAP[counter];
            val.AFR = AFR[counter];
            val.IAT = IAT[counter];
            val.engineTemp = engineTemp[counter];
            val.oilPressure = oilPressure[counter];
            val.oilTemp = oilTemp[counter];
            val.fuelTemp = fuelTemp[counter];
            val.xAccel = xAccel[counter];
            val.yAccel = yAccel[counter];
            val.zAccel = zAccel[counter];
            val.roll = roll[counter];
            val.pitch = pitch[counter];
            val.yaw = yaw[counter];
            val.longitude = longitude[counter];
            val.latitiude = latitude[counter];
            val.speed = speed[counter];
            val.distance = distance[counter];
            //sendData(val, sock);
            sendto(sock, (struct Data *)(&val), sizeof(val), 0, (const struct sockaddr *)&serv_addr, sizeof(serv_addr));
            startTime = clock();
            check = 0;
            counter++;
        }
    }

    int rc = close(sock);
    return 0;
}