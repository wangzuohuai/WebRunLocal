import Vue from 'vue';
import Router from 'vue-router';

Vue.use(Router);

const routes = [{
		path: '/',
		name: 'index',
		component: () => import('./view/index'),
		meta: {
			title: '首页'
		},
	}
];

// add route path
routes.forEach(route => {
	route.path = route.path || '/' + (route.name || '');
});

const router = new Router({
	routes
});

router.beforeEach((to, from, next) => {
	const title = to.meta && to.meta.title;
	if (title) {
		document.title = title;
	}
	next();
});

export {
	router
};
