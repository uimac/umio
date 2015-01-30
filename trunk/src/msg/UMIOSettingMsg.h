/**
 *
 * @file UMIOSettingMsg.h
 *
 * @author tori31001 at gmail.com
 *
 *
 * Copyright (C) 2014 Kazuma Hatta
 * Licensed under the MIT or GPL Version 3 licenses. 
 *
 */
#ifdef WITH_MSGPACK
	typedef std::map<unsigned int, int> SerializableMap;
	
	typedef msgpack::type::tuple<
	SerializableMap,
	SerializableMap,
	SerializableMap,
	SerializableMap
	> setting_msg_type;

	template <typename Stream>
	void msgpack_pack(msgpack::packer<Stream>& o) const {
		SerializableMap exp;
		{
			ExpSettingMap::const_iterator it = exp_prop_map_.begin();
			for (; it != exp_prop_map_.end(); ++it) {
				exp[it->first] = !!it->second;
			}
		}
		SerializableMap imp;
		{
			ImpSettingMap::const_iterator it = imp_prop_map_.begin();
			for (; it != imp_prop_map_.end(); ++it) {
				imp[it->first] = !!it->second;
			}
		}
		SerializableMap blexp;
		{
			UMExpSettingMap::const_iterator it = bl_exp_prop_map_.begin();
			for (; it != bl_exp_prop_map_.end(); ++it) {
				blexp[it->first] = !!it->second;
			}
		}
		SerializableMap blimp;
		{
			UMImpSettingMap::const_iterator it = bl_imp_prop_map_.begin();
			for (; it != bl_imp_prop_map_.end(); ++it) {
				blimp[it->first] = !!it->second;
			}
		}
		o.pack(setting_msg_type(exp, imp, blexp, blimp));
	}
	
	void msgpack_unpack(setting_msg_type v) {
		SerializableMap exp = v.get<0>();
		{
			SerializableMap::const_iterator it = exp.begin();
			for (; it != exp.end(); ++it) {
				exp_prop_map_[static_cast<EExpSettingType>(it->first)] = !!it->second;
			}
		}
		SerializableMap imp = v.get<1>();
		{
			SerializableMap::const_iterator it = exp.begin();
			for (; it != exp.end(); ++it) {
				imp_prop_map_[static_cast<EImpSettingType>(it->first)] = !!it->second;
			}
		}
		SerializableMap blexp = v.get<2>();
		{
			SerializableMap::const_iterator it = blexp.begin();
			for (; it != blexp.end(); ++it) {
				bl_exp_prop_map_[static_cast<EUMExpSettingType>(it->first)] = !!it->second;
			}
		}
		SerializableMap blimp = v.get<3>();
		{
			SerializableMap::const_iterator it = blimp.begin();
			for (; it != blimp.end(); ++it) {
				bl_imp_prop_map_[static_cast<EUMImpSettingType>(it->first)] = !!it->second;
			}
		}
	}
#endif //WITH_MSGPACK
