#ifndef CUDA_OPERATIONAL_MODEL_H_
#define CUDA_OPERATIONAL_MODEL_H_

#include <stdio.h>
#include <stdlib.h>

/*
 * CUDA types
 */
typedef enum cublasstatus {
  CUBLAS_STATUS_SUCCESS,
  CUBLAS_STATUS_NOT_INITIALIZED,
  CUBLAS_STATUS_ALLOC_FAILED,
  CUBLAS_STATUS_INVALID_VALUE,
  CUBLAS_STATUS_ARCH_MISMATCH,
  CUBLAS_STATUS_MAPPING_ERROR,
  CUBLAS_STATUS_EXECUTION_FAILED,
  CUBLAS_STATUS_INTERNAL_ERROR,
  CUBLAS_STATUS_NOT_SUPPORTED,
  CUBLAS_STATUS_LICENSE_ERROR
} custatusResult;

typedef enum cublasstatus cublasStatus_t;

typedef struct cublashandle {
} cublasHandle_t;

typedef enum cublasoperation {
  CUBLAS_OP_N,
  CUBLAS_OP_T,
  CUBLAS_OP_C
} cuoperation;

typedef enum cublasoperation cublasOperation_t;

/*
 * CUDA methods
 */

/*
 * This function initializes the CUBLAS library and creates a handle to an
 * opaque structure holding the CUBLAS library context. It allocates hardware
 * resources on the host and device and must be called prior to making any other
 * CUBLAS library calls. The CUBLAS library context is tied to the current CUDA
 * device. To use the library on multiple devices, one CUBLAS handle needs to be
 * created for each device. Furthermore, for a given device, multiple CUBLAS
 * handles with different configuration can be created. Because cublasCreate
 * allocates some internal resources and the release of those resources by
 * calling cublasDestroy will implicitly call cublasDeviceSynchronize, it is
 * recommended to minimize the number of cublasCreate/cublasDestroy occurences.
 * For multi-threaded applications that use the same device from different
 * threads, the recommended programming model is to create one CUBLAS handle per
 * thread and use that CUBLAS handle for the entire life of the thread.
 */
cublasStatus_t cublasCreate(cublasHandle_t *handle) {
  return CUBLAS_STATUS_SUCCESS;
}

/*
 * This function releases hardware resources used by the CUBLAS library. This
 * function is usually the last call with a particular handle to the CUBLAS
 * library. Because cublasCreate allocates some internal resources and the
 * release of those resources by calling cublasDestroy will implicitly call
 * cublasDeviceSynchronize, it is recommended to minimize the number of
 * cublasCreate/cublasDestroy occurences.
 */
cublasStatus_t cublasDestroy(cublasHandle_t handle) {
  return CUBLAS_STATUS_SUCCESS;
}

/*
 * This function copies a tile of rows x cols elements from a matrix A in
 * host memory space to a matrix B in GPU memory space. It is assumed that each
 * element requires storage of elemSize bytes and that both matrices are stored
 * in column-major format, with the leading dimension of the source matrix A and
 * destination matrix B given in lda and ldb, respectively. The leading
 * dimension indicates the number of rows of the allocated matrix, even if only
 * a submatrix of it is being used. In general, B is a device pointer that
 * points to an object, or part of an object, that was allocated in GPU memory
 * space via cublasAlloc().
 */
cublasStatus_t cublasSetMatrix(int rows, int cols, int elemSize, const void *A,
                               int lda, void *B, int ldb) {
  return CUBLAS_STATUS_SUCCESS;
}

/*
 * This function copies a tile of rows x cols elements from a matrix A in GPU
 * memory space to a matrix B in host memory space. It is assumed that each
 * element requires storage of elemSize bytes and that both matrices are stored
 * in column-major format, with the leading dimension of the source matrix A and
 * destination matrix B given in lda and ldb, respectively. The leading
 * dimension indicates the number of rows of the allocated matrix, even if only
 * a submatrix of it is being used. In general, A is a device pointer that
 * points to an object, or part of an object, that was allocated in GPU memory
 * space via cublasAlloc().
 */
cublasStatus_t cublasGetMatrix(int rows, int cols, int elemSize, const void *A,
                               int lda, void *B, int ldb) {
  return CUBLAS_STATUS_SUCCESS;
}

cublasStatus_t cublasSetVector(int n, int elemSize, const void *A, int lda,
                               void *B, int ldb) {
  return CUBLAS_STATUS_SUCCESS;
}

cublasStatus_t cublasGetVector(int n, int elemSize, const void *A, int lda,
                               void *B, int ldb) {
  return CUBLAS_STATUS_SUCCESS;
}

/*
 * see CUDA documentation:
 * http://developer.download.nvidia.com/compute/cuda/2_0/docs/CUBLAS_Library_2.0.pdf
 *
 * We assume that:
 * (1) CUBLAS library was initialized
 * (2) Function was able to allocate reduction buffer
 */
cublasStatus_t cublasIsamin(cublasHandle_t handle, int n, const float *x,
                            int incx, int *result) {
  int i, j;
  result = 0;
  for (i = 0; i < n; i++) {
    j = 1 + (i - 1) * incx;
    if (x[result[0]] >= x[j]) {
      result[0] = j;
    }
    return CUBLAS_STATUS_SUCCESS;
  }
  return CUBLAS_STATUS_EXECUTION_FAILED;
}

/*
 * We assume that:
 * (1) CUBLAS library was initialized
 * (2) Function was able to allocate reduction buffer
 * (3) Device supports double precision
 */
cublasStatus_t cublasIdamin(cublasHandle_t handle, int n, const double *x,
                            int incx, int *result) {
  int i, j;
  result = 0;
  for (i = 0; i < n; i++) {
    j = 1 + (i - 1) * incx;
    if (x[result[0]] >= x[j]) {
      result[0] = j;
    }
    return CUBLAS_STATUS_SUCCESS;
  }
  return CUBLAS_STATUS_EXECUTION_FAILED;
}

cublasStatus_t cublasSasum(cublasHandle_t handle, int n, const float *x,
                           int incx, float *result) {
  int i, j;
  float calculate = 0;
  result = 0;
  for (i = 0; i < n; i++) {
    j = 1 + (i - 1) * incx;
    calculate = x[j] + calculate;
  }
  result[0] = calculate;
  return CUBLAS_STATUS_SUCCESS;
}

cublasStatus_t cublasDasum(cublasHandle_t handle, int n, const double *x,
                           int incx, double *result) {
  int i, j;
  double calculate = 0;
  result = 0;
  for (i = 0; i < n; i++) {
    j = 1 + (i - 1) * incx;
    calculate = x[j] + calculate;
  }
  result[0] = calculate;
  return CUBLAS_STATUS_SUCCESS;
}

/*
 * We assume that:
 * (1) CUBLAS library was initialized
 * (2) Function was able to allocate reduction buffer
 */
cublasStatus_t cublasIsamax(cublasHandle_t handle, int n, const float *x,
                            int incx, int *result) {
  int i, j;
  result = 0;
  for (i = 0; i < n; i++) {
    j = 1 + (i - 1) * incx;
    if (x[result[0]] <= x[j]) {
      result[0] = j;
    }
    return CUBLAS_STATUS_SUCCESS;
  }
  return CUBLAS_STATUS_EXECUTION_FAILED;
}

/*
 * We assume that:
 * (1) CUBLAS library was initialized
 * (2) Function was able to allocate reduction buffer
 * (3) Device supports double precision
 */
cublasStatus_t cublasIdamax(cublasHandle_t handle, int n, const double *x,
                            int incx, int *result) {
  int i, j;
  result = 0;
  for (i = 0; i < n; i++) {
    j = 1 + (i - 1) * incx;
    if (x[result[0]] <= x[j]) {
      result[0] = j;
    }
    return CUBLAS_STATUS_SUCCESS;
  }
  return CUBLAS_STATUS_EXECUTION_FAILED;
}

cublasStatus_t cublasSscal(cublasHandle_t handle, int n, const float *alpha,
                           float *x, int incx) {
  int i, j;
  for (i = 0; i < n; i++) {
    j = 1 + (i - 1) * incx;
    x[j] = ((float)alpha[0]) * x[j];
  }
  return CUBLAS_STATUS_SUCCESS;
}

cublasStatus_t cublasDscal(cublasHandle_t handle, int n, const double *alpha,
                           double *x, int incx) {
  int i, j;
  for (i = 0; i < n; i++) {
    j = 1 + (i - 1) * incx;
    x[j] = ((double)alpha[0]) * x[j];
  }
  return CUBLAS_STATUS_SUCCESS;
}

cublasStatus_t cublasSswap(cublasHandle_t handle, int n, float *x, int incx,
                           float *y, int incy) {
  int i, j, k;
  float aux = 0;
  for (i = 0; i < n; i++) {
    k = 1 + (i - 1) * incx;
    j = 1 + (i - 1) * incy;
    aux = y[j];
    y[j] = x[k];
    x[k] = y[j];
  }
  return CUBLAS_STATUS_SUCCESS;
}

cublasStatus_t cublasDswap(cublasHandle_t handle, int n, double *x, int incx,
                           double *y, int incy) {
  int i, j, k;
  double aux = 0;
  for (i = 0; i < n; i++) {
    k = 1 + (i - 1) * incx;
    j = 1 + (i - 1) * incy;
    aux = y[j];
    y[j] = x[k];
    x[k] = y[j];
  }
  return CUBLAS_STATUS_SUCCESS;
}

cublasStatus_t cublasSdot(cublasHandle_t handle, int n, const float *x,
                          int incx, const float *y, int incy, float *result) {
  int i, j, k;
  float aux = 0;
  for (i = 0; i < n; i++) {
    k = 1 + (i - 1) * incx;
    j = 1 + (i - 1) * incy;
    aux = y[j] * x[k] + aux;
  }
  result[0] = aux;
  return CUBLAS_STATUS_SUCCESS;
}

cublasStatus_t cublasDdot(cublasHandle_t handle, int n, const double *x,
                          int incx, const double *y, int incy, double *result) {
  int i, j, k;
  double aux = 0;
  for (i = 0; i < n; i++) {
    k = 1 + (i - 1) * incx;
    j = 1 + (i - 1) * incy;
    aux = y[j] * x[k] + aux;
  }
  result[0] = aux;
  return CUBLAS_STATUS_SUCCESS;
}

cublasStatus_t cublasScopy(cublasHandle_t handle, int n, const float *x,
                           int incx, float *y, int incy) {
  int i, j, k;
  for (i = 0; i < n; i++) {
    k = 1 + (i - 1) * incx;
    j = 1 + (i - 1) * incy;
    y[j] = x[k];
  }
  return CUBLAS_STATUS_SUCCESS;
}

cublasStatus_t cublasDcopy(cublasHandle_t handle, int n, const double *x,
                           int incx, double *y, int incy) {
  int i, j, k;
  for (i = 0; i < n; i++) {
    k = 1 + (i - 1) * incx;
    j = 1 + (i - 1) * incy;
    y[j] = x[k];
  }
  return CUBLAS_STATUS_SUCCESS;
}

cublasStatus_t cublasSgemm(cublasHandle_t handle, cublasOperation_t transa,
                           cublasOperation_t transb, int m, int n, int k,
                           const float *alpha, const float *A, int lda,
                           const float *B, int ldb, const float *beta, float *C,
                           int ldc) {
  int counterX = 0;
  int counterY = 0;
  int counterZ = 0;
  float result = 0;
  if ((transa == CUBLAS_OP_N) && (transb == CUBLAS_OP_N)) {
    result = 0;
    for (counterZ = 0; counterZ < m; counterZ++) {
      result = 0;
      for (counterY = 0; counterY < n; counterY++) {
        result = 0;
        for (counterX = 0; counterX < k; counterX++) {
          result = (A[counterX + counterZ * k] * B[counterX*n +counterY]) + result;
          //printf("result: %.6f A: %.6f B: %.6f indexA: %d indexB: %d \n", result, A[counterX + counterZ * k], B[counterX*n +counterY], counterX + counterZ * k, counterX*n +counterY);
        }
        C[counterZ*n + counterY] = alpha[0] * result + beta[0] * C[counterZ*n + counterY];
      }
    }
  } else if ((transa == CUBLAS_OP_N) && (transb == CUBLAS_OP_T)) {
    result = 0;
    for (counterZ = 0; counterZ < m; counterZ++) {
      result = 0;
      for (counterY = 0; counterY < n; counterY++) {
        result = 0;
        for (counterX = 0; counterX < k; counterX++) {
          result = (A[counterX + counterZ*k] * B[counterX + counterY*k]) + result;
        }
        C[counterZ*n + counterY] = alpha[0] * result + beta[0] * C[counterZ*n + counterY];
      }
    }
  } else if ((transa == CUBLAS_OP_T) && (transb == CUBLAS_OP_N)) {
    result = 0;
    for (counterZ = 0; counterZ < m; counterZ++) {
      result = 0;
      for (counterY = 0; counterY < n; counterY++) {
        result = 0;
        for (counterX = 0; counterX < k; counterX++) {
          result = (A[counterX * m + counterZ] * B[counterX*n + counterY]) + result;
          //printf("result: %.6f A: %.6f B: %.6f indexA: %d indexB: %d \n", result, A[counterX + counterZ * k], B[counterX*n +counterY], counterX + counterZ * k, counterX*n +counterY);
        }
        C[counterZ *n + counterY] = alpha[0] * result + beta[0] * C[counterZ*n + counterY];
      }
    }
  } else if ((transa == CUBLAS_OP_T) && (transb == CUBLAS_OP_T)) {
    result = 0;
    for (counterZ = 0; counterZ < m; counterZ++) {
      for (counterY = 0; counterY < n; counterY++) {
        result = 0;
        for (counterX = 0; counterX < k; counterX++) {
          result = (A[counterX * k + counterZ] * B[counterX]) + result;
        }
        C[counterZ * m] = alpha[0] * result + beta[0] * C[counterZ * m];
      }
    }
  }
  return CUBLAS_STATUS_SUCCESS;
}

cublasStatus_t cublasDgemm(cublasHandle_t handle, cublasOperation_t transa,
                           cublasOperation_t transb, int m, int n, int k,
                           const double *alpha, const double *A, int lda,
                           const double *B, int ldb, const double *beta,
                           double *C, int ldc) {
  int counterX, counterY, counterZ;
  double result = 0;

  if ((transa == CUBLAS_OP_N) && (transb == CUBLAS_OP_N)) {
    result = 0;
    for (counterZ = 0; counterZ < m; counterZ++) {
      for (counterY = 0; counterY < n; counterY++) {
        result = 0;
        for (counterX = 0; counterX < k; counterX++) {
          result = (A[counterX + counterZ * k] * B[counterX * n + counterY]) +
                   result;
        }
        C[counterY + counterZ * m] =
            alpha[0] * result + beta[0] * C[counterY + counterZ * m];
      }
    }
  } else if ((transa == CUBLAS_OP_N) && (transb == CUBLAS_OP_T)) {
    result = 0;
    for (counterZ = 0; counterZ < m; counterZ++) {
      for (counterY = 0; counterY < n; counterY++) {
        result = 0;
        for (counterX = 0; counterX < k; counterX++) {
          result = (A[counterX + counterZ * k] * B[counterX + counterY * n]) +
                   result;
        }
        C[counterY + counterZ * m] =
            alpha[0] * result + beta[0] * C[counterY + counterZ * m];
      }
    }
  } else if ((transa == CUBLAS_OP_T) && (transb == CUBLAS_OP_N)) {
    result = 0;
    for (counterZ = 0; counterZ < m; counterZ++) {
      for (counterY = 0; counterY < n; counterY++) {
        result = 0;
        for (counterX = 0; counterX < k; counterX++) {
          result = (A[counterX * k + counterZ] * B[counterX * n + counterY]) +
                   result;
        }
        C[counterY + counterZ * m] =
            alpha[0] * result + beta[0] * C[counterY + counterZ * m];
      }
    }
  } else if ((transa == CUBLAS_OP_T) && (transb == CUBLAS_OP_T)) {
    result = 0;
    for (counterZ = 0; counterZ < m; counterZ++) {
      for (counterY = 0; counterY < n; counterY++) {
        result = 0;
        for (counterX = 0; counterX < k; counterX++) {
          result = (A[counterX * k + counterZ] * B[counterX + counterY * n]) +
                   result;
        }
        C[counterY + counterZ * m] =
            alpha[0] * result + beta[0] * C[counterY + counterZ * m];
      }
    }
  }
  return CUBLAS_STATUS_SUCCESS;
}

cublasStatus_t cublasSaxpy(cublasHandle_t handle, int n, const float *alpha,
                           const float *x, int incx, float *y, int incy) {
  short int i;
  int k, j;
  for (i = 0; i < n; i++) {
    k = 1 + (i - 1) * incx;
    j = 1 + (i - 1) * incy;
    y[j] = ((float)alpha[0]) * x[k] + y[j];
  }
  return CUBLAS_STATUS_SUCCESS;
}

cublasStatus_t cublasDaxpy(cublasHandle_t handle, int n, const double *alpha,
                           const double *x, int incx, double *y, int incy) {
  short int i;
  int k, j;
  for (i = 0; i < n; i++) {
    k = 1 + (i - 1) * incx;
    j = 1 + (i - 1) * incy;
    y[j] = ((double)alpha[0]) * x[k] + y[j];
  }
  return CUBLAS_STATUS_SUCCESS;
}

#endif
