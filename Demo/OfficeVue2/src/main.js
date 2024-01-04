import Vue from 'vue';
import App from './App';
import {
	router
} from './router';
import ElementUI from 'element-ui';
import 'element-ui/lib/theme-chalk/index.css';
Vue.use(ElementUI);
let qs = require('qs');
Vue.prototype.$qs = qs;
new Vue({
	router,
	el: '#app',
	render: h => h(App)
});
