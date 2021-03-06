////////////////////////////////////////////////////////////////////////////////
//                               --  THYME  --                                //
////////////////////////////////////////////////////////////////////////////////
//
//  Project Name:: Thyme
//
//          File:: ARCHIVEFILESYSTEM.H
//
//        Author:: OmniBlade
//
//  Contributors:: 
//
//   Description:: Interface for local file system implementations.
//
//       License:: Thyme is free software: you can redistribute it and/or 
//                 modify it under the terms of the GNU General Public License 
//                 as published by the Free Software Foundation, either version 
//                 2 of the License, or (at your option) any later version.
//
//                 A full copy of the GNU General Public License can be found in
//                 LICENSE
//
////////////////////////////////////////////////////////////////////////////////
#ifdef _MSC_VER
#pragma once
#endif // _MSC_VER

#ifndef _ARCHIVEFILESYSTEM_H_
#define _ARCHIVEFILESYSTEM_H_

#include "subsysteminterface.h"
#include "hooker.h"

class File;
class ArchiveFile;

#define TheArchiveFileSystem (Make_Global<ArchiveFileSystem*>(0x00A2BA00))

class ArchiveFileSystem : public SubsystemInterface 
{
    public:
        virtual ArchiveFile *Open_Archive_File(char const *filename) = 0;
        virtual void Close_Archive_File(char const *filename) = 0;
        virtual void Close_All_Archives() = 0;
        virtual File *Open_File(char const *filename, int mode);
        virtual void Close_All_Files() = 0;
        virtual bool Does_File_Exist(char const *filename);
        virtual void Load_Archives_From_Dir(AsciiString dir, AsciiString filter, bool read_subdirs) = 0;
        virtual void Load_Into_Dir_Tree(ArchiveFile const *file, AsciiString const &dir, bool unk);
};

#endif // _ARCHIVEFILESYSTEM_H_
