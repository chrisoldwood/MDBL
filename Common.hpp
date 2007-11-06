////////////////////////////////////////////////////////////////////////////////
//! \file   Common.hpp
//! \brief  Wrapper to include the most common MDBL library headers.
//! \author Chris Oldwood

// Check for previous inclusion
#ifndef MDBL_COMMON_HPP
#define MDBL_COMMON_HPP

#if _MSC_VER > 1000
#pragma once
#endif

////////////////////////////////////////////////////////////////////////////////
// Library documentation
//
//! \namespace MDBL
//! \brief     The Memory Database Library namespace.

////////////////////////////////////////////////////////////////////////////////
// Standard headers.

#include <Core/Common.hpp>		// Core library common headers.
#include <WCL/Common.hpp>		// Windows C++ library common headers.

////////////////////////////////////////////////////////////////////////////////
// Library headers.

#include "FwdDecls.hpp"

#endif // MDBL_COMMON_HPP
