#ifndef CGUICAMERA_H
#define CGUICAMERA_H

#include <QObject>
#include <QMutex>
#include <QImage>

// Include files to use the pylon API.
#include <pylon/PylonIncludes.h>
#include <pylon/BaslerUniversalInstantCamera.h>

class CGuiCamera : public QObject
        , public Pylon::CImageEventHandler             // Allows you to get notified about images received and grab errors.
        , public Pylon::CConfigurationEventHandler     // Allows you to get notified about device removal.
        , public Pylon::CCameraEventHandler            // Allows you to get notified about camera events and GenICam node changes.

{
    Q_OBJECT
public:
    CGuiCamera();
    virtual ~CGuiCamera();

    // Attributes
    void SetUserHint( int userHint );
    int GetUserHint() const;

    void SetInvertImage( bool enable );

    const QImage& GetImage() const;

    bool IsCameraDeviceRemoved() const;
    bool IsOpen() const;
    bool IsGrabbing() const;
    bool IsSingleShotSupported() const;

    // For GUI
    Pylon::IIntegerEx& GetExposureTime();
    Pylon::IIntegerEx& GetGain();
    Pylon::IEnumParameterT<Basler_UniversalCameraParams::PixelFormatEnums>& GetPixelFormat();
    Pylon::IEnumParameterT<Basler_UniversalCameraParams::TriggerModeEnums>& GetTriggerMode();
    Pylon::IEnumParameterT<Basler_UniversalCameraParams::TriggerSourceEnums>& GetTriggerSource();
    QMutex* GetBmpLock() const;

    // Statistics
    uint64_t GetGrabbedImages() const;
    uint64_t GetGrabbedImagesDiff();
    uint64_t GetGrabErrors() const;

    // Operations
    void Open( const Pylon::CDeviceInfo& deviceInfo );
    void Close();

    void SingleGrab();
    void ContinuousGrab();
    void StopGrab();

    void ExecuteSoftwareTrigger();

    // Pylon::CImageEventHandler functions
    virtual void OnImagesSkipped( Pylon::CInstantCamera& camera, size_t countOfSkippedImages );
    virtual void OnImageGrabbed( Pylon::CInstantCamera& camera, const Pylon::CGrabResultPtr& grabResult );

    // Pylon::CConfigurationEventHandler functions
    virtual void OnAttach( Pylon::CInstantCamera& camera );
    virtual void OnAttached( Pylon::CInstantCamera& camera );
    virtual void OnDetach( Pylon::CInstantCamera& camera );
    virtual void OnDetached( Pylon::CInstantCamera& camera );
    virtual void OnDestroy( Pylon::CInstantCamera& camera );
    virtual void OnDestroyed( Pylon::CInstantCamera& camera );
    virtual void OnOpen( Pylon::CInstantCamera& camera );
    virtual void OnOpened( Pylon::CInstantCamera& camera );
    virtual void OnClose( Pylon::CInstantCamera& camera );
    virtual void OnClosed( Pylon::CInstantCamera& camera );
    virtual void OnGrabStart( Pylon::CInstantCamera& camera );
    virtual void OnGrabStarted( Pylon::CInstantCamera& camera );
    virtual void OnGrabStop( Pylon::CInstantCamera& camera );
    virtual void OnGrabStopped( Pylon::CInstantCamera& camera );
    virtual void OnGrabError( Pylon::CInstantCamera& camera, const char* errorMessage );
    virtual void OnCameraDeviceRemoved( Pylon::CInstantCamera& camera );

    // Pylon::CCameraEventHandler function
    virtual void OnCameraEvent( Pylon::CInstantCamera& camera, intptr_t userProvidedId, GenApi::INode* pNode );

signals:
    void NewGrabResult( int userHint );
    void StateChanged( int userHint, bool isGrabbing );
    void DeviceRemoved( int userHint );
    void NodeUpdated( int userHint, GenApi::INode* pNode );

private:
    // Statistical values
    uint64_t m_cntGrabbedImages;
    uint64_t m_cntSkippedImages;
    uint64_t m_cntGrabErrors;
    uint64_t m_cntLastGrabbedImages;

    // Used to store the index of the camera.
    int m_userHint;
    // Will be set to toggle image processing on or off.
    bool m_invertImage;
    // Protects members.
    mutable QMutex m_MemberLock;
    // Protects the converted image.
    mutable QMutex m_imageLock;

private:
    // The pylon camera object.
    Pylon::CBaslerUniversalInstantCamera m_camera;
    // The grab result retrieved from the camera.
    Pylon::CGrabResultPtr m_ptrGrabResult;
    // The grab result as a QImage to be displayed on the screen.
    QImage m_image;
    // The format converter to convert a grab result into a QImage.
    Pylon::CImageFormatConverter m_formatConverter;

    // Configurations to apply when starting single or continuous grab.
    Pylon::CAcquireSingleFrameConfiguration m_singleConfiguration;
    Pylon::CAcquireContinuousConfiguration m_continuousConfiguration;

    // Smart pointer to camera features
    Pylon::CIntegerParameter m_exposureTime;
    Pylon::CIntegerParameter m_gain;
};

#endif // CGUICAMERA_H
