//----------------------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "StringThis.h"

namespace Azul
{
	StringThis::StringThis(Mesh::Name status)
	{
		switch(status)
		{
		case Mesh::Name::CUBE:
			strcpy_s(this->buffer, BUFFER_SIZE, STRING_ME(Mesh::CUBE));
			break;

		case Mesh::Name::PYRAMID:
			strcpy_s(this->buffer, BUFFER_SIZE, STRING_ME(Mesh::PYRAMID));
			break;

		case Mesh::Name::CRATE:
			strcpy_s(this->buffer, BUFFER_SIZE, STRING_ME(Mesh::CRATE));
			break;

		case Mesh::Name::SPHERE:
			strcpy_s(this->buffer, BUFFER_SIZE, STRING_ME(Mesh::SPHERE));
			break;

		case Mesh::Name::NOT_INITIALIZED:
			strcpy_s(this->buffer, BUFFER_SIZE, STRING_ME(Mesh::NOT_INITIALIZED));
			break;

		case Mesh::Name::NULL_MESH:
			strcpy_s(this->buffer, BUFFER_SIZE, STRING_ME(Mesh::NULL_MESH));
			break;

		default:
			assert(false);
		}
	}

	StringThis::StringThis(Camera::Name status)
	{
		switch(status)
		{
		case Camera::Name::CAMERA_0:
			strcpy_s(this->buffer, BUFFER_SIZE, STRING_ME(Camera::CAMERA_0));
			break;

		case Camera::Name::CAMERA_1:
			strcpy_s(this->buffer, BUFFER_SIZE, STRING_ME(Camera::CAMERA_1));
			break;

		case Camera::Name::NOT_INITIALIZED:
			strcpy_s(this->buffer, BUFFER_SIZE, STRING_ME(Camera::NOT_INITIALIZED));
			break;

		case Camera::Name::NullCamera:
			strcpy_s(this->buffer, BUFFER_SIZE, STRING_ME(Camera::NullCamera));
			break;

		default:
			assert(false);
		}
	}

	StringThis::StringThis(ShaderObject::Name status)
	{
		switch(status)
		{
		case ShaderObject::Name::NullShader:
			strcpy_s(this->buffer, BUFFER_SIZE, STRING_ME(ShaderObject::NullShader));
			break;

		case ShaderObject::Name::ColorByVertex:
			strcpy_s(this->buffer, BUFFER_SIZE, STRING_ME(ShaderObject::ColorByVertex));
			break;

		case ShaderObject::Name::FlatTexture:
			strcpy_s(this->buffer, BUFFER_SIZE, STRING_ME(ShaderObject::FlatTexture));
			break;

		case ShaderObject::Name::LightTexture:
			strcpy_s(this->buffer, BUFFER_SIZE, STRING_ME(ShaderObject::LightTexture));
			break;

		case ShaderObject::Name::Uninitialized:
			strcpy_s(this->buffer, BUFFER_SIZE, STRING_ME(ShaderObject::Uninitialized));
			break;

		default:
			assert(false);
		}
	}

	StringThis::StringThis(TextureObject::Name status)
	{
		switch(status)
		{
		case TextureObject::Name::NullTexture:
			strcpy_s(this->buffer, BUFFER_SIZE, STRING_ME(TextureObject::NullShader));
			break;

		case TextureObject::Name::Duckweed:
			strcpy_s(this->buffer, BUFFER_SIZE, STRING_ME(TextureObject::Duckweed));
			break;

		case TextureObject::Name::Uninitialized:
			strcpy_s(this->buffer, BUFFER_SIZE, STRING_ME(TextureObject::Uninitialized));
			break;

		default:
			assert(false);
		}
	}


	StringThis::operator char *()
	{
		return this->buffer;
	}
}

// --- End of File ---
