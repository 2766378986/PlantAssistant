cmake_minimum_required(VERSION 3.14)
project(PlantAssistant VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Sql Quick Core Gui QuickWidgets Multimedia)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Sql Quick Core Gui QuickWidgets Multimedia)



set(PROJECT_SOURCES
    ./src/main.cpp
    ./src/Window/plantassistant.cpp
    ./src/Window/plantassistant.h
    ./src/Manager/Manager.cpp
    ./src/Manager/Manager.h
    ./src/Window/plantassistant.ui
    ./src/Window/plantassistant.qrc
    src/Widgets/PlantInformationManager/plantinformationmanage.h
    src/Widgets/PlantInformationManager/plantinformationmanage.cpp
    src/Widgets/PlantInformationManager/plantinformationmanage.ui
    src/Window/login.h
    src/Window/login.cpp
    src/Window/login.ui
    src/Dialogs/MessageBox/pmessagebox.h
    src/Dialogs/MessageBox/pmessagebox.cpp
    src/Dialogs/MessageBox/pmessagebox.ui
    src/CoreData/User/User.cpp
    src/Widgets/PlantCard/plantcard.h
    src/Widgets/PlantCard/plantcard.cpp
    src/Widgets/PlantCard/plantcard.ui
    src/CoreData/Plant/PlantInfo.cpp
    src/Dialogs/AddPlantCard/addplantcarddialog.h
    src/Dialogs/AddPlantCard/addplantcarddialog.cpp
    src/Dialogs/AddPlantCard/addplantcarddialog.ui
    src/Dialogs/PopUpDialog/popupdialog.h
    src/Dialogs/PopUpDialog/popupdialog.cpp
    src/Dialogs/PopUpDialog/popupdialog.ui
    src/Widgets/CareRemind/careremind.h
    src/Widgets/CareRemind/careremind.cpp
    src/Widgets/CareRemind/careremind.ui
    src/Widgets/TaskWidget/taskwidget.h
    src/Widgets/TaskWidget/taskwidget.cpp
    src/Widgets/TaskWidget/taskwidget.ui
    src/Dialogs/AddRemindTask/addremindtask.h
    src/Dialogs/AddRemindTask/addremindtask.cpp
    src/Dialogs/AddRemindTask/addremindtask.ui
    src/Widgets/SelectCard/selectcard.h
    src/Widgets/SelectCard/selectcard.cpp
    src/Widgets/SelectCard/selectcard.ui
    src/Widgets/SelectCard/selectcardmanager.h
    src/Widgets/SelectCard/selectcardmanager.cpp
    src/Widgets/SelectCard/selectcardmanager.ui
    src/CoreData/CareRemind/Remind.cpp
    src/Widgets/TaskWidget/taskwidgetmanager.h
    src/Widgets/TaskWidget/taskwidgetmanager.cpp
    src/Widgets/TaskWidget/taskwidgetmanager.ui
)

qt_add_resources(PlantAssistant ./src/Window/plantassistant.qrc)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(PlantAssistant
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
else()
    add_executable(PlantAssistant
        ${PROJECT_SOURCES}
    )
endif()

target_link_libraries(PlantAssistant PRIVATE 
    Qt${QT_VERSION_MAJOR}::Widgets 
    Qt${QT_VERSION_MAJOR}::Sql 
    Qt${QT_VERSION_MAJOR}::Quick 
    Qt${QT_VERSION_MAJOR}::Core 
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::QuickWidgets
    Qt${QT_VERSION_MAJOR}::Multimedia
)


if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.PlantAssistant)
endif()
set_target_properties(PlantAssistant PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(PlantAssistant)
endif()
