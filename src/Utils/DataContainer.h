/*
 * DataContainer.h
 *
 *  Created on: 19.4.2011
 *      Author: macjariel
 */

#ifndef __DATACONTAINER_H__
#define __DATACONTAINER_H__

#include "Setup/common.h"

namespace Utils
{

	class DataContainer
	{
	public:
		/**
		 * Constructs a container that will own no memory block.
		 */
		DataContainer();

		/**
		 * Constructs a container that will own specified memory block.
		 * @param data Pointer to the memory block.
		 * @param size Size of the memory block.
		 */
		DataContainer(uint8_t* data, uint32_t size);

		/**
		 * Constructs a container that will own the memory block previously owned by other.
		 * The other container will no longer own a memory block.
		 * @param other
		 */
		DataContainer(DataContainer&& other);

		/**
		 * Destroys the container and frees the memory block that it owns.
		 */
		virtual ~DataContainer();

		/**
		 * Returns byte in the owned memory block at the specified position. This method does not
		 * perform any range checking.
		 * @param index The index to the memory block.
		 * @return Byte at the index in the owned memory block.
		 */
		const uint8_t& operator[](uint32_t index) const;

		/**
		 * Move operator.
		 */
		DataContainer& operator=(DataContainer&& other);

		/**
		 * Returns the size of the owned memory block.
		 */
		uint32_t size() const;

	private:
		/**
		 * Copy constructor and copy operator= are disabled.
		 */
		/**@{*/
		DataContainer(const DataContainer& other);
		DataContainer& operator=(const DataContainer& other);
		/**@}*/

		uint8_t* mData;
		uint32_t mSize;
	};

} // namespace Utils

#endif /* __DATACONTAINER_H__ */
