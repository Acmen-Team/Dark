#pragma once

namespace Dark {
  /*
  class AsyncFileOperation
  {
  public:
    AsyncFileOperation(File file, size_t position);
    AsyncFileOperation(const AsyncFileOperation& other);
    AsyncFileOperation& operator=(const AsyncFileOperation& other);
    ~AsyncFileOperation();

      /// Returns whether or not the asynchronous operation has finished
    bool HasFinished(void) const;

    /// Waits until the asynchronous operation has finished. Returns the number of transferred bytes.
    size_t WaitUntilFinished(void) const;

    /// Cancels the asynchronous operation
    void Cancel(void);

  private:
    HANDLE m_file;

    ReferenceCountedItem<OVERLAPPED>* m_overlapped;
  };
  */

  class File
  {
  public:
    virtual ~File(){};
    ////////////////////
    // Synchronously API
    ////////////////////

    // Synchronously reads from the file into a buffer. Returns the number of bytes read, or 0 if the operation failed.
    virtual size_t Read(void* buffer, size_t length) = 0;
    // Synchronously writes from a buffer into the file. Returns the number of bytes written, or 0 if the operation failed.
    virtual size_t Write(const void* buffer, size_t length) = 0;

    // Seeks to the desired position.
    virtual void Seek(size_t position) = 0;
    // Seeks to the end of the file.
    virtual void SeekToEnd() = 0;

    // Skips a certain amount of bytes.
    virtual void Skip(size_t bytes) = 0;
    // Returns the current position in the file, or INVALID_SET_FILE_POINTER (0xFFFFFFFF) if the operation failed.
    virtual size_t Tell() const = 0;

    /////////////////////
    // Asynchronously API
    /////////////////////

    // Asynchronously reads from the file into a buffer
    virtual void ReadAsync(void* buffer, size_t length) = 0;
    // Asynchronously writes from a buffer into the file
    virtual void WriteAsync(const void* buffer, size_t length) = 0;

    // Waits until the asynchronous operation has finished
    virtual void WaitForAsync(void) = 0;
    // Returns the number of bytes transferred in the asynchronous operation
    virtual size_t GetAsyncBytesTransferred() const = 0;

  private:
  };
} // namespace Dark