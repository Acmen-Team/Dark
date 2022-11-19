#pragma once

#include <glm/glm.hpp>

namespace Dark {

  // Maximum capacity of camera bookmark
  constexpr int BOOKMARK_SIZE = 3;

  // Camera bookmark structure
  struct CameraBookmark
  {
    // Id. A unique identifier for a bookmark
    int m_Id;
    // Bookmark name
    std::string m_Name;
    // Bookmark position
    glm::vec3 m_Bookmark;
    // A flag. When true: Enable the bookmark. When false: Disable the bookmark
    bool m_Flag;

    CameraBookmark(int id = -1, std::string name = "", glm::vec3 bookmark = {0.0f, 0.0f, 0.0f}, bool flag = false)
    {
      m_Id       = id;
      m_Name     = name;
      m_Bookmark = bookmark;
      m_Flag     = flag;
    }
    ~CameraBookmark() = default;
  };

  class OrthographicCamera
  {
  public:
    OrthographicCamera(float left, float right, float bottom, float top);
    void SetProjection(float left, float right, float bottom, float top);

    const glm::vec3& GetPosition() const { return m_Position; }
    void SetPosition(const glm::vec3& position)
    {
      m_Position = position;
      RecalculateViewMatrix();
    }
    float GetRotation() const { return m_Rotation; }
    void SetRotation(float rotation)
    {
      m_Rotation = rotation;
      RecalculateViewMatrix();
    }

    const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
    const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
    const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }

    /**
     *  Sets the orthographic camera's camera bookmark.
     *
     *  @param position     Bookmark`s position
     */
    CameraBookmark SetCameraBookmark(const glm::vec3 position);

    /**
     *  Deletes the camera bookmark. Delete a bookmark by its Id
     */
    void DeleteCameraBookmark(const CameraBookmark bookmark);

    /**
     *  Returns the orthographic camera's camera bookmark.
     *  TODO The argument to this function should have been the bookmark Id(future version)
     *
     *  @param index    Stores the index subscript of an array of bookmarks
     *  @return The camera bookmark.
     */
    CameraBookmark GetCameraBookmark(const int index);

  private:
    void RecalculateViewMatrix();

  private:
    glm::mat4 m_ProjectionMatrix;
    glm::mat4 m_ViewMatrix;
    glm::mat4 m_ViewProjectionMatrix;

    glm::vec3 m_Position = {0.0f, 0.0f, 0.0f};
    float m_Rotation     = 0.0f;

    // Record all bookmarks of camera
    CameraBookmark m_CameraBookmark[BOOKMARK_SIZE];
  };

} // namespace Dark