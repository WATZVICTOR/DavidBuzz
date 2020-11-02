//
//  Cliente.swift
//  Cliente
//
//  Created by Fernando on 30/09/2020.
//  Copyright © 2020 Victor. All rights reserved.
//

import Foundation
import Network

class Cliente {
    let connection: ClientConnection
    let host: NWEndpoint.Host
    let port: NWEndpoint.Port

    init(host: String, port: UInt16) {
        self.host = NWEndpoint.Host(host)
        self.port = NWEndpoint.Port(rawValue: port)!
        let nwConnection = NWConnection(host: self.host, port: self.port, using: .tcp)
        connection = ClientConnection(nwConnection: nwConnection)
    }

    func start() {
        print("Client started \(host) \(port)")
        connection.didStopCallback = didStopCallback(error:)
        connection.start()
    }

    func stop() {
        connection.stop()
    }

    func send(data: Data) {
        connection.send(data: data)
    }

    func didStopCallback(error: Error?) {
        if error == nil {
            exit(EXIT_SUCCESS)
        } else {
            exit(EXIT_FAILURE)
        }
    }
}
