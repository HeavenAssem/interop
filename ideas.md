# Microservice use-case

## Compatibility nodes / modules

### Important notes
This is totally optional functionality that is meant to ease backward compatibility with outdated but yet supported nodes out of reach of developer, such as mobile or desktop clients of client-server application. <br/>
Versions here are representing only incompatible implementations of same object and thus must be issued only in case if "by-design" backward compatibility is impossible, like:
* field type change
* required field removal
<br/>

If changes are safe, such as: <br/>
* addition of new field
* optional field removal
<br/>

But it's worth to mention that in the end only developer can tell if change of API object is safe, because framework is not aware of object's fields' semantics. A framework will try to compare metadata of different versions of API object in order to detect problems that will trigger errors on framework level, but there could be false negative cases (no problems detected but translation is needed) if, for example, developer have changed units of values passed in field, but not the type of field. <br/>
So in summary: don't forget to use your head - the framework is not a silver bullet.

### General
Convertion will be done by:
1. converting new object to some intermediate representation (IR)
2. convert that IR to old version IR
3. convert old version IR to concrete data representation format (JSON, Protobuf, etc.)

Thus, using IR, we will be able to convert anything to anything by just adding new data representation backend and without modifying data translation code

### Compatibility module
A special kind of module that knows all previous versions of some objects and knows how to convert objects of current version to older ones. <br/>
Internal structure of convertions is user-defined. <br/>
For example a user can do incremental structure: <br/>
\<version 5> -> <version 4> -> <version 3> -> <version 2> -> <version 1>
and in order to convert object of 5-th version to 1-st one, 4 sequential convertions will be performed. <br/>
This is very resiliant, intuitive simple and easy to use approach, but could be inefficient. Thus, another approach is possible: <br/>
\<version 5> -> <version 4> <br/>
\<version 5> -> <version 3> <br/>
\<version 5> -> <version 2> <br/>
\<version 5> -> <version 1> <br/>
This approach is fastest in terms of processing, but in case of API object change will require to rewrite all translations, which is sad. <br/>
User can mix those approaches: <br/>
\<version 8> -> <version 7>
\<version 8> -> <version 6>
\<version 8> -> <version 5> -> <version 4> -> <version 3> <br/>
\<version 3> -> <version 2> <br/>
\<version 3> -> <version 1> <br/>
In this case interop framework will find shortest sequence of convertions using given translation options and pair of versions

### Compatibility node
A node that consists only from compatibility modules and operates as proxy between data producer and consumer. Must be specified in producer node config as compatibility node. <br/>
Could be useful if you want to prepare translations first before updating data producers or be able to easily turn of translations.
