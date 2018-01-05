# MosaicFramework
Cross - language, modular, event driven framework for application development

Mosaic is a application architecture framework that aims to provide easy interoperability between different languages (planned):

- C/C++
- D
- JavaScript
- Python
- (maybe) Haskell

Application consists of one or more nodes.  
Node consists of one or more modules.  
Module consists of types, functions and events.  
Node corresponds to process.  
Modules of same application can interact with each other via different buses, such as:

- Shared memory
- Sockets
- WebSockets
- etc...

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
