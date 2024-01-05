 
 
module.exports = {
	outputDir: 'dist',
	lintOnSave: false,
	publicPath: process.env.NODE_ENV === 'production' ? './' : '/',
	css: {
		 
	},
	productionSourceMap: false,
	// devServer: {
	// 	proxy: { //配置跨域
	// 		'/api': {
	// 			target:process.env.VUE_APP_API_URL, //这里后台的地址模拟的;应该填写你们真实的后台接口
	// 			changOrigin: true, //允许跨域
	// 			pathRewrite: {				
	// 				'^/api': ''
	// 			}
	// 		},
	// 	}
	// },
};
