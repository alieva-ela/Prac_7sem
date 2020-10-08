#include <addon.hxx>
#include <iostream>
#include <string>
#include <algorithm>
#include <cstdlib>
#include <ctime>

#include <com/sun/star/frame/Desktop.hpp>
#include <com/sun/star/frame/XComponentLoader.hpp>
#include <com/sun/star/frame/XFrame.hpp>
#include <com/sun/star/frame/XController.hpp>
#include <com/sun/star/frame/XModel.hpp>

#include <com/sun/star/container/XEnumerationAccess.hpp>
#include <com/sun/star/container/XContentEnumerationAccess.hpp>
#include <com/sun/star/container/XEnumeration.hpp>
#include <com/sun/star/container/XNameAccess.hpp>
#include <com/sun/star/container/XIndexAccess.hpp>

#include <com/sun/star/bridge/XUnoUrlResolver.hpp>
#include <com/sun/star/uno/XComponentContext.hpp>
#include <com/sun/star/uno/XInterface.hpp>

#include <com/sun/star/text/XTextDocument.hpp>
#include <com/sun/star/text/XText.hpp>
#include <com/sun/star/text/XTextTablesSupplier.hpp>
#include <com/sun/star/text/XTextContent.hpp>
#include <com/sun/star/text/XTextRange.hpp>
#include <com/sun/star/text/XTextRangeCompare.hpp>
#include <com/sun/star/text/XTextTable.hpp>


#include <com/sun/star/lang/IndexOutOfBoundsException.hpp>
#include <com/sun/star/lang/XMultiServiceFactory.hpp>
#include <com/sun/star/lang/XSingleServiceFactory.hpp>

#include <com/sun/star/view/XSelectionSupplier.hpp>

#include <com/sun/star/table/XCell.hpp>
#include <com/sun/star/table/XCell.hpp>
#include <com/sun/star/table/XTableRows.hpp>
#include <com/sun/star/table/XCellRange.hpp>
#include <com/sun/star/table/XTable.hpp>

#include <cppuhelper/supportsservice.hxx>
#include <cppuhelper/bootstrap.hxx>

#include <rtl/ustring.hxx>

using namespace cppu;
using namespace com::sun::star::uno;
using namespace com::sun::star::frame;
using namespace com::sun::star::table;
using namespace com::sun::star::lang;
using namespace com::sun::star::beans;
using namespace com::sun::star::text;
using namespace com::sun::star::view;
using namespace com::sun::star::bridge;
using namespace com::sun::star::container;
using com::sun::star::util::URL;

using ::rtl::OUString;
using ::rtl::OUStringToOString;
using ::rtl::OString;

#define SERVICE_NAME "com.sun.star.frame.ProtocolHandler"


void CreateDocAndTable(Reference<XFrame> &xFrame) {
    Reference<XComponentLoader> rComponentLoader(xFrame, UNO_QUERY);

    Reference<XComponent> xComponent = rComponentLoader->loadComponentFromURL(
        OUString::createFromAscii("private:factory/swriter"),
        OUString::createFromAscii("_default"),
        0,
        Sequence<::com::sun::star::beans::PropertyValue>());
    
    Reference<XTextDocument> xTextDocument (xComponent, UNO_QUERY);
    Reference <XMultiServiceFactory> Document(xTextDocument, UNO_QUERY);

    Reference<XText> xText = xTextDocument->getText();
    Reference<XTextCursor> xTextCursor = xText->createTextCursor();

    int countTables = 2 + std::rand() % 7;

    for (int i = 0; i < countTables; i++) {
        xTextCursor->gotoEnd(false);

        std::string st = std::string("\nTable ") + std::to_string(i + 1);
        xTextCursor->setString(OUString::createFromAscii(st.c_str()));

        Reference<XTextTable> xTable(Document->createInstance(OUString::createFromAscii("com.sun.star.text.TextTable")), UNO_QUERY);
        
        if (!xTable.is()) {
            printf("Error creation XTextTable interface !\n");
            return;
        }

        int rows = 3 + std::rand() % 8;
        int columns = 3 + std::rand() % 4;
        xTable->initialize(rows, columns);
        Reference<XTextContent> xTextContent(xTable, UNO_QUERY);

        // Insert the table into the document
        xText->insertTextContent(xText->getEnd(), xTextContent, (unsigned char) 0);

        for (int y = 0; y < rows; y++) {
            for (int x = 0; x < columns; x++) {
                std::string index = (char) ('A' + x) + std::to_string(y + 1);
                Reference<XCell> xCell = xTable->getCellByName(OUString::createFromAscii(index.c_str()));
                Reference<XText> xTextCell(xCell, UNO_QUERY);
                Reference<XTextCursor> xTextCursor1 = xTextCell->createTextCursor();

                std::string cell_val = "row_" +std::to_string(x + 1) + " col_" + std::to_string(y + 1);

                xTextCursor1->setString(OUString::createFromAscii(cell_val.c_str()));
            }
        }
    }
}

void TransposeTable(Reference<XTextTable>& xTable) {
    Reference<XTextTableCursor> cur = xTable->createCursorByCellName(OUString::createFromAscii("A1")); 
        
    int c = 1;
    while (cur->goDown(1, false)) c++;

    int r = 1;
    while (cur->goRight(1, false)) r++;
    int max_size = std::min(c, r);

    for (int y = 0; y < max_size; y++) {
        for (int x = y; x < max_size; x++) {
            std::string indexCell_1 = (char) ('A' + x) + std::to_string(y + 1);
            Reference<XCell> xCell_1 = xTable->getCellByName(OUString::createFromAscii(indexCell_1.c_str()));
            Reference<XText> xTextCell_1(xCell_1, UNO_QUERY);

            std::string indexCell_2 = (char) ('A' + y) + std::to_string(x + 1);
            Reference<XCell> xCell_2 = xTable->getCellByName(OUString::createFromAscii(indexCell_2.c_str()));
            Reference<XText> xTextCell_2(xCell_2, UNO_QUERY);

            auto tmp = xTextCell_1->getString();
            xTextCell_1->setString(xTextCell_2->getString());
            xTextCell_2->setString(tmp);
        }
    }
}

void TransposeAllDocument(Reference<XFrame>& xFrame) {
    Reference < XController > xFrameController = xFrame->getController();
    Reference<XTextDocument> xTextDocument(xFrameController->getModel(), UNO_QUERY);
    
    Reference<XTextTablesSupplier> xTables(xTextDocument, UNO_QUERY);
    Reference<XNameAccess> xNameTables = xTables->getTextTables();

    auto n = xNameTables->getElementNames();
    for (auto& i : n) {
        Any t = xNameTables->getByName(i);
        Reference<XTextTable> xTable(t, UNO_QUERY);
        if (not xTable.is()){
            std::cerr << "Some trouble connect to table" << std::endl;
            return;
        }
        TransposeTable(xTable);
    }
}



// XDispatch implementer class "DateTimeWriterDispatchImpl" methods
void SAL_CALL AddonDispatchImpl::dispatch(const URL& aURL, const Sequence <PropertyValue>& lArgs) throw (RuntimeException) {
    if (aURL.Protocol.equalsAscii("inco.niocs.test.protocolhandler:")) {
        printf("DEBUG>>> CreatorAndTableProcessingDispatchImpl::dispatch() called. this = %p, command = %s\n", this,
        OUStringToOString( aURL.Path, RTL_TEXTENCODING_ASCII_US ).getStr()); fflush(stdout);
        if (aURL.Path.equalsAscii("Transpose table")) {
            TransposeAllDocument(mxFrame);
        } else if (aURL.Path.equalsAscii("Open")) {
            CreateDocAndTable(mxFrame);
        }
    }
}

void SAL_CALL AddonDispatchImpl::addStatusListener(const Reference<XStatusListener>& xControl, const URL& aURL) throw (RuntimeException) {
}

void SAL_CALL AddonDispatchImpl::removeStatusListener(const Reference<XStatusListener>& xControl, const URL& aURL) throw (RuntimeException) {
}



// ProtocolHandler implementation "Addon" class methods

void SAL_CALL Addon::initialize(const Sequence<Any>& aArguments) throw (Exception, RuntimeException) {
    Reference<XFrame> xFrame;
    if (aArguments.getLength()) {
        aArguments[0] >>= xFrame;
        mxFrame = xFrame;
    }
}

Reference<XDispatch> SAL_CALL Addon::queryDispatch(const URL& aURL, const ::rtl::OUString& sTargetFrameName, sal_Int32 nSearchFlags) throw (RuntimeException) {
    Reference<XDispatch> xRet;
    if (aURL.Protocol.equalsAscii("inco.niocs.test.protocolhandler:")) {
        if (aURL.Path.equalsAscii("Transpose")) {
            xRet = new AddonDispatchImpl(mxFrame, mxContext);
        } else if (aURL.Path.equalsAscii("Insert")) {
            xRet = new AddonDispatchImpl(mxFrame, mxContext);
        }
    }

    return xRet;
}


Sequence<Reference<XDispatch>> SAL_CALL Addon::queryDispatches(const Sequence<DispatchDescriptor>& seqDescripts) throw (RuntimeException) {
    sal_Int32 nCount = seqDescripts.getLength();
    Sequence<Reference<XDispatch>> lDispatcher(nCount);

    for (sal_Int32 i = 0; i < nCount; ++i)
        lDispatcher[i] = queryDispatch(seqDescripts[i].FeatureURL, seqDescripts[i].FrameName, seqDescripts[i].SearchFlags);

    return lDispatcher;
}


// Helper functions for the implementation of UNO component interfaces.
OUString Addon_getImplementationName() throw (RuntimeException) {
    return OUString(IMPLEMENTATION_NAME);
}

Sequence<::rtl::OUString> SAL_CALL Addon_getSupportedServiceNames() throw (RuntimeException) {
    Sequence <::rtl::OUString> aRet(1);
    ::rtl::OUString* pArray = aRet.getArray();
    pArray[0] = OUString(SERVICE_NAME);
    return aRet;
}

Reference<XInterface> SAL_CALL Addon_createInstance(const Reference<XComponentContext> & rContext) throw (Exception) {
    return (cppu::OWeakObject*) new Addon(rContext);
}

// Implementation of the recommended/mandatory interfaces of a UNO component.
// XServiceInfo
::rtl::OUString SAL_CALL Addon::getImplementationName() throw (RuntimeException) {
    return Addon_getImplementationName();
}

sal_Bool SAL_CALL Addon::supportsService(const ::rtl::OUString& rServiceName) throw (RuntimeException) {
    return cppu::supportsService(this, rServiceName);
}

Sequence<::rtl::OUString> SAL_CALL Addon::getSupportedServiceNames() throw (RuntimeException) {
    return Addon_getSupportedServiceNames();
}
