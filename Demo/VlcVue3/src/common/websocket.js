export default class websocket {
	constructor(url, options) {
		this.instance = null
		this.token = null
		this.isConnected = false
		this.url = url
		this.options = options || this.defaultOptions()
		if (this.options) {
			this.reconnectEnabled = options.reconnectEnabled || false
			if (this.reconnectEnabled) {
				this.reconnectInterval = options.reconnectInterval
			}
			// Token
			//this.token = options?.token || null
		}
		// These methods should be defined by components
		this.onOpen = null
		this.onMessage = null
		this.onClose = null
		this.onError = null
	}

	defaultOptions() {
		return {
			reconnectEnabled: false,
			reconnectInterval: 0,
			token: null
		}
	}

	connect() {
		const token = this.token || null
		let url = this.url
		if (token !== null) {
			url += `?token=${token}`
		}
		this.instance = new WebSocket(url)

		// Socket event listeners
		// Each event handler also calls the corresponding class method,
		// which can be defined by the component
		this.instance.onopen = () => {
			this.isConnected = true
			if (typeof this.onOpen === 'function') {
				this.onOpen()
			}
		}
		this.instance.onmessage = (msg) => {
			if (typeof this.onMessage === 'function') {
				this.onMessage(msg)
			}
		}
		this.instance.onclose = (evt) => {
			this.isConnected = false
			if (typeof this.onClose === 'function') {
				this.onClose(evt)
			}
			if (this.reconnectEnabled) {
				this.reconnect()
			}
		}
		this.instance.onerror = (evt) => {
			if (typeof this.onError === 'function') {
				this.onError(evt)
			}
		}
	}

	disconnect() {	 
		try {
			this.instance.close()
		} catch (e) {
			console.warn(`${e} ${this.instance}`)
		}
		delete this.instance
	}

	reconnect() {		 
		try {
			this.instance.close()
		} catch (e) {
			console.warn(`${e} ${this.instance}`)
		}
		delete this.instance
		setTimeout(() => {
			this.connect()
		}, this.reconnectInterval)
	}

	sendObj(data) {

		if (this.instance.readyState === this.instance.OPEN) {
			//若是ws开启状态

			this.instance.send(JSON.stringify(data))
		} else if (this.instance.readyState === this.instance.CONNECTING) {
			// 若是 正在开启状态，则等待1s后重新调用
			setTimeout(() => {
				this.sendObj(data);
			}, 1000);
		} else {
			// 若未开启 ，则等待1s后重新调用
			setTimeout(() => {
				this.sendObj(data);
			}, 1000);
		}
		
	}

	removeListeners() {
		// removeListeners
		this.onOpen = null
		this.onMessage = null
		this.onClose = null
		this.onError = null
	}
}
