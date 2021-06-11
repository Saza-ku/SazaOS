/**
 * @file pci.cpp
 * @author Saza-ki
 * @brief PCI バス制御のプログラムを集めたファイル
 * @version 0.1
 * @date 2021-05-01
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "pci.hpp"

#include "asmfunc.h"

namespace {
	using namespace pci;

	// #@@range_begin(make_address)
	/** 
	 * @brief CONFIG_ADDRESS 用の 32 ビット整数を生成する
	 * @param (bus) バス番号
	 * @param (device) デバイス番号
	 * @param (function) ファンクション番号
	 * @param (reg_addr) レジスタオフセット
	 * @return CONFIG_ADDRESS に書き込む 32 ビット整数
	*/
	uint32_t MakeAddress(uint8_t bus, uint8_t device,
											 uint8_t function, uint8_t reg_addr) {
		// ラムダ式 : 関数の中で関数を定義できる
		// x を bits だけ左シフト
		auto shl = [](uint32_t x, unsigned int bits) {
			return x << bits;
		};

		// 最上位ビット Enable は 1 にしておく
		// 下位 2 ビットは 0 になるようにする
		return shl(1, 31)
				| shl(bus, 16)
				| shl(device, 11)
				| shl(function, 8)
				| (reg_addr & 0xfcu);
	}
	// #@@range_end(make_address)

}

namespace pci {
	// #@@range_begin(config_addr_data)
	void WriteAddress(uint32_t address) {
		IoOut32(kConfigAddress, address);
	}

	void WriteData(uint32_t value) {
		IoOut32(kConfigData, value);
	}

	uint32_t ReadData() {
		return IoIn32(kConfigData);
	}

	uint16_t ReadVendorId(uint8_t bus, uint8_t device, uint8_t function) {
		WriteAddress(MakeAddress(bus, device, function, 0x00));
		return ReadData() & 0xffffu;
	}
	// #@@range_end(config_addr_data)

	uint16_t ReadDeviceId(uint8_t bus, uint8_t device, uint8_t function) {
		WriteAddress(MakeAddress(bus, device, function, 0x00));
		return ReadData() >> 16;
	}

	uint8_t ReadHeaderType(uint8_t bus, uint8_t device, uint8_t function) {
		WriteAddress(MakeAddress(bus, device, function, 0x0c));
		return (ReadData() >> 16) & 0xffu;
	}

	uint32_t ReadClassCode(uint8_t bus, uint8_t device, uint8_t function) {
		WriteAddress(MakeAddress(bus, device, function, 0x08));
		return ReadData();
	}

	uint32_t ReadBusNumbers(uint8_t bus, uint8_t device, uint8_t function) {
		WriteAddress(MakeAddress(bus, device, function, 0x18));
		return ReadData();
	}

	bool isSingleFunctionDevice(uint8_t header_type) {
		return (header_type & 0x80u) == 0;
	}
	
	// PCI バスに繋がったデバイスを探索する
	Error ScanAllBus() {
		num_device = 0;

		auto header_type = ReadHeaderType(0, 0, 0);
		if (IsSingleFunctionDevice(header_type)) {
			return ScanBus(0);
		}

		for (uint8_t function = 1; function < 8; ++function) {
			if (ReadVendorId(0, 0, function) == 0xffffu) {
				continue;
			}
			if (auto err = ScanBus(function)) {
				return err;
			}
		}

		return Error::kSuccess;
	}
}