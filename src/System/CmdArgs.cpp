#include <FRONTIER/System/CmdArgs.hpp>

namespace fm
{
	/////////////////////////////////////////////////////////////
	CmdArgs::CmdArgs() {}

	/////////////////////////////////////////////////////////////
	CmdArgs::CmdArgs(int argc,char *argv[])
	{
		setArgs(argc,argv);
	}

	/////////////////////////////////////////////////////////////
	void CmdArgs::setArgs(int argc,char *argv[])
	{
		m_args.assign(argv,argv + argc);
	}

	/////////////////////////////////////////////////////////////
	void CmdArgs::addArg(fm::String arg)
	{
		m_args.push_back(arg);
	}

	/////////////////////////////////////////////////////////////
	fm::Size CmdArgs::getArgCount() const
	{
		return m_args.size();
	}

	/////////////////////////////////////////////////////////////
	fm::String CmdArgs::getArg(fm::Size index) const
	{
		return m_args[index];
	}

	/////////////////////////////////////////////////////////////
	fm::Result CmdArgs::processArgs() const
	{
		// skip first
		for (fm::Size id = 1;id < getArgCount();++id)
		{
			fm::String arg = getArg(id);
			while (arg.size() && (arg[0] == '-' || arg[0] == '/'))
			{
				arg = arg.substr(1);
			}

			auto cbit = m_callbacks.find(arg);

			if (cbit == m_callbacks.end())
			{
				return fm::Result("CMDError",fm::Result::OPFailed,"UnknownSwitch","processSwitch",__FILE__,__LINE__,"unknown switch " + arg.str());
			}
			else
			{
				fm::Result res;

				if (!cbit->second(res,id))
				{
					if (!res) return res;
				}
			}
		}

		return fm::Result();
	}
	
	/////////////////////////////////////////////////////////////
	CmdArgs &CmdArgs::forwSwitch(fm::String argSwitch,fm::String forwSwitch)
	{
		m_callbacks[forwSwitch] = m_callbacks[argSwitch];
		
		return *this;
	}
	
	/////////////////////////////////////////////////////////////
	CmdArgs &CmdArgs::addSwitch(fm::String argSwitch,fm::Delegate<bool> callback)
	{
		m_callbacks[argSwitch] = [&,callback](fm::Result &res,fm::Size &id) -> bool {
			
			++id;
			bool cb = callback();

			if (!cb)
				res = fm::Result("CMDError",fm::Result::OPFailed,"UnaccpetedValue","processSwitch",__FILE__,__LINE__,"Unacceptable value was given to " + argSwitch.str());

			return cb;
		};
		
		return *this;
	}
}

#ifndef FRONTIER_DONT_INCLUDE_INL
	#include <FRONTIER/System/CmdArgs.inl>
#endif
