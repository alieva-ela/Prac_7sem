#ifndef INCLUDED_TABLE_TRANSPOSITOR
#define INCLUDED_TABLE_TRANSPOSITOR

#include <com/sun/star/lang/XInitialization.hpp>
#include <com/sun/star/frame/XDispatchProvider.hpp>
#include <com/sun/star/lang/XServiceInfo.hpp>
#include <cppuhelper/implbase4.hxx>

#define IMPLEMENTATION_NAME "org.openoffice.Office.table.transpositor"

namespace com
{
    namespace sun
    {
        namespace star
        {
            namespace frame
            {
                class XFrame;
            }
            namespace awt
            {
                class XToolkit;
            }
            namespace uno
            {
                class XComponentContext;
            }
        }
    }
}

class Addon : public cppu::WeakImplHelper4
<
    com::sun::star::frame::XDispatchProvider,
    com::sun::star::frame::XDispatch,
    com::sun::star::lang::XInitialization,
    com::sun::star::lang::XServiceInfo
>

{
private:
    ::com::sun::star::uno::Reference< ::com::sun::star::uno::XComponentContext > mxContext;
    ::com::sun::star::uno::Reference< ::com::sun::star::frame::XFrame > mxFrame;
    ::com::sun::star::uno::Reference< ::com::sun::star::awt::XToolkit > mxToolkit;

public:
    Addon(const ::com::sun::star::uno::Reference<::com::sun::star::uno::XComponentContext> &rxContext) : mxContext(rxContext) {}

    // XDispatchProvider
    virtual ::com::sun::star::uno::Reference<::com::sun::star::frame::XDispatch>
        SAL_CALL queryDispatch(
                const ::com::sun::star::util::URL& aURL,
                const ::rtl::OUString& sTargetFrameName, 
                sal_Int32 nSearchFlags)
            throw(::com::sun::star::uno::RuntimeException);

    
    virtual ::com::sun::star::uno::Sequence <::com::sun::star::uno::Reference<::com::sun::star::frame::XDispatch>>
        SAL_CALL queryDispatches(
                const ::com::sun::star::uno::Sequence <::com::sun::star::frame::DispatchDescriptor>& seqDescriptor)
            throw(::com::sun::star::uno::RuntimeException);

    
    // XDispatch
    virtual void 
        SAL_CALL dispatch(
                const ::com::sun::star::util::URL& aURL,
                const ::com::sun::star::uno::Sequence<::com::sun::star::beans::PropertyValue>& lArgs)
            throw (::com::sun::star::uno::RuntimeException);
    
    
    virtual void 
        SAL_CALL addStatusListener(
                const ::com::sun::star::uno::Reference<::com::sun::star::frame::XStatusListener>& xControl,
                const ::com::sun::star::util::URL& aURL) 
            throw (::com::sun::star::uno::RuntimeException);
    

    virtual void
        SAL_CALL removeStatusListener(
                const ::com::sun::star::uno::Reference<::com::sun::star::frame::XStatusListener>& xControl,
                const ::com::sun::star::util::URL& aURL) 
            throw (::com::sun::star::uno::RuntimeException);


    // XInitialization
    virtual void 
        SAL_CALL initialize(
                const ::com::sun::star::uno::Sequence<::com::sun::star::uno::Any>& aArguments)
            throw (::com::sun::star::uno::Exception, ::com::sun::star::uno::RuntimeException);


    // XServiceInfo
    virtual ::rtl::OUString SAL_CALL getImplementationName()
        throw (::com::sun::star::uno::RuntimeException);

    virtual sal_Bool SAL_CALL supportsService(const ::rtl::OUString& ServiceName)
        throw (::com::sun::star::uno::RuntimeException);

    virtual ::com::sun::star::uno::Sequence<::rtl::OUString> SAL_CALL getSupportedServiceNames(  )
        throw (::com::sun::star::uno::RuntimeException);
};


::rtl::OUString Addon_getImplementationName() throw (::com::sun::star::uno::RuntimeException);


sal_Bool SAL_CALL Addon_supportsService(const ::rtl::OUString& ServiceName) throw ( ::com::sun::star::uno::RuntimeException );


::com::sun::star::uno::Sequence<::rtl::OUString> 
    SAL_CALL Addon_getSupportedServiceNames()
    throw (::com::sun::star::uno::RuntimeException);

::com::sun::star::uno::Reference<::com::sun::star::uno::XInterface>
    SAL_CALL Addon_createInstance( 
            const ::com::sun::star::uno::Reference<::com::sun::star::uno::XComponentContext> & rContext)
        throw (::com::sun::star::uno::Exception);

#endif
