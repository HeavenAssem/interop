# MosaicFramework
Cross - language, modular, event driven framework for application development

Mosaic is a application architecture framework that aims to provide an easy interoperability on multiple levels:
 * Language level: ability to write different parts of system using different languages
 * Architecture level: ability to choose how to run different parts of system
 * Data level: ability to choose data format for serialization/deserialization (if used)

Planned languages to support:
 * C/C++
 * D
 * JavaScript
 * Python
 * (maybe) Haskell
 
 Planned architecture options (Transports):
  * Direct (native to native only): modules are acting like plugins, running in same process and directly calling functions and trigger events with minimal overhead
  * Shared memory: modules run in separate processes and interact via shared memory
  * Sockets: interprocess communication via sockets
  * Websockets
  * HTTP
  
 Planned data representation options:
  * JSON
  * msgpack
  * protobuf
  * flatbuffers

Application consists of one or more nodes.  
Node consists of one or more modules.  
Module consists of types, functions and events.  
Node corresponds to process.  
Modules of same application can interact with each other via different transports, described above

Modules of the same node interact using same API as if they were in different nodes.  
Modules of the same node interact with each other with minimum overhead, without bus.  
Modules can have (optional) dependencies on another modules.  

Nodes of same application can run on different machines, with different OSs, with different CPU architectures.  

Application node structure is how modules are organized into nodes and how nodes are then organized into application.  
Application node structure is determined at runtime (application start-up).  
Application node structure is described in JSON configs of nodes.  
Application node structure change does not imply any rebuild, only restart of application.  

### Very distant future
Online modules library.  
Automatic download of missing modules mentioned in config or required by another module at application start-up.  

### Maybe
Users can implement their own buses
