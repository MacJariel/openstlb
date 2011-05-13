/*
 * MemoryReader.h
 *
 *  Created on: 9.4.2011
 *      Author: macjariel
 */

#ifndef __MEMORYREADER_H__
#define __MEMORYREADER_H__

#include "IReader.h"

namespace Utils
{
	class DataContainer;

	class MemoryReader: public IReader
	{
	public:
		MemoryReader(const DataContainer& data);

		/**
		 * Constructs a MemoryReader that can read data from specified data container.
		 * @param data The container that will be read.
		 * @param delegateOwnership Whether the container will be moved to the reader.
		 */
		MemoryReader(DataContainer&& data, bool moveContainer);

		/**
		 * Destroys the MemoryReader.
		 */
		virtual ~MemoryReader();

		/**
		 * Reads size bytes of data and stores it to buffer.
		 */
		virtual IReader& read(uint8_t* buffer, uint32_t size);

		/**
		 * Reads size bytes of data and returns it in a data container.
		 * @param size Number of bytes to be read.
		 * @return A data container
		 */
		virtual Utils::DataContainer read(uint32_t size);

		/**
		 * Reads size bytes long number in little endian encoding and stores it to buffer.
		 */
		virtual IReader& readNumber(uint8_t* buffer, uint32_t size);

		/**
		 * Returns the position in the read object.
		 */
		virtual uint32_t pos() const;

		/**
		 * Sets the position in the read object.
		 */
		virtual void setPos(uint32_t newPos);

		/**
		 * Skips the next size bytes. Negative number skips backwards.
		 */
		virtual void skip(int32_t size);

		/**
		 * Returns the size of the read object.
		 */
		virtual uint32_t size() const;

	private:
		DataContainer* mData;
		bool mOwnsData;
		uint32_t mPos;
	};

}
#endif /* __MEMORYREADER_H__ */
