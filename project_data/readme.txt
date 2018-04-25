файлы:
brewing.json, reflux.json, distillation.json, heater.json, configs.json
для теста получения данных без сервера

package.json
для создания релиза «data» — копируется index.htm и сжимаются, копируются script и style в gzip (требуется Node.js)

//& copyfiles index.htm ../data/
//& html-minifier index.htm --output ../data/index.htm --collapse-whitespace

