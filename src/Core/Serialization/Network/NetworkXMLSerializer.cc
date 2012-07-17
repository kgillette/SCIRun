/*
   For more information, please see: http://software.sci.utah.edu

   The MIT License

   Copyright (c) 2012 Scientific Computing and Imaging Institute,
   University of Utah.

   License for the specific language governing rights and limitations under
   Permission is hereby granted, free of charge, to any person obtaining a
   copy of this software and associated documentation files (the "Software"),
   to deal in the Software without restriction, including without limitation
   the rights to use, copy, modify, merge, publish, distribute, sublicense,
   and/or sell copies of the Software, and to permit persons to whom the
   Software is furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included
   in all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
   OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
   THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
   FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
   DEALINGS IN THE SOFTWARE.
*/

#include <Core/Serialization/Network/NetworkXMLSerializer.h>
#include <Core/Serialization/Network/NetworkDescriptionSerialization.h>
#include <Core/Dataflow/Network/Network.h> //TODO: need network factory??
#include <Core/Dataflow/Network/ModuleInterface.h>
#include <fstream>
#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/foreach.hpp>

using namespace SCIRun::Domain::Networks;

NetworkXMLConverter::NetworkXMLConverter(ModuleFactoryHandle moduleFactory, ModuleStateFactoryHandle stateFactory)
  : moduleFactory_(moduleFactory), stateFactory_(stateFactory)
{
}

NetworkHandle NetworkXMLConverter::from_xml_data(const NetworkXML& data)
{
  NetworkHandle network(new Network(moduleFactory_, stateFactory_));
  //BOOST_FOREACH(const ConnectionDescriptionXML&)


  return network;
}

NetworkXMLHandle NetworkXMLConverter::to_xml_data(const NetworkHandle& network)
{
  NetworkXMLHandle xmlData(new NetworkXML);
  Network::ConnectionDescriptionList conns = network->connections();
  BOOST_FOREACH(ConnectionDescription& desc, conns)
    xmlData->connections.push_back(ConnectionDescriptionXML(desc));
  for (size_t i = 0; i < network->nmodules(); ++i)
  {
    ModuleHandle module = network->module(i);
    xmlData->modules[module->get_id()] = module->get_info();
  }

  return xmlData;
}

void NetworkXMLSerializer::save_xml(const NetworkXML& data, const std::string& filename)
{
  std::ofstream ofs(filename.c_str());
  save_xml(data, ofs);
}

void NetworkXMLSerializer::save_xml(const NetworkXML& data, std::ostream& ostr)
{
  if (!ostr.good())
    return;
  boost::archive::xml_oarchive oa(ostr);
  oa << boost::serialization::make_nvp("network", data);
}

NetworkXMLHandle NetworkXMLSerializer::load_xml(const std::string& filename)
{
  std::ifstream ifs(filename.c_str());
  return load_xml(ifs);
}

NetworkXMLHandle NetworkXMLSerializer::load_xml(std::istream& istr)
{
  if (!istr.good())
    return NetworkXMLHandle();
  boost::archive::xml_iarchive ia(istr);
  NetworkXMLHandle nh(new NetworkXML);
  ia >> BOOST_SERIALIZATION_NVP(*nh);
  return nh;
}
