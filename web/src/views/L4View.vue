<template>
    <div class="flex w-full gap-2">
        <div>
            <h2 class="text-center">TCP</h2>
            <div class="flex w-full gap-1 items-start">
                <TwoColTable column1="Source Port" column2="Times" :data="tcpStats['srcPortCounter']" />
                <TwoColTable column1="Destination Port" column2="Times" :data="tcpStats['dstPortCounter']" />
            </div>
        </div>
        <div>
            <h2 class="text-center">UDP</h2>
            <div class="flex w-full gap-1 items-start">
                <TwoColTable column1="Source Port" column2="Times" :data="udpStats['srcPortCounter']" />
                <TwoColTable column1="Destination Port" column2="Times" :data="udpStats['dstPortCounter']" />
            </div>
        </div>
    </div>
</template>

<script>
import TwoColTable from '../components/TwoColTable.vue'
export default {
    name: 'L4View',
    data() {
        return {
            tcpStats: {},
            udpStats: {},
        }
    },
    components: {
        TwoColTable
    },
    methods: {
        preprocessData(data) {
            // sort and filter port number < 49152
            let tmp = Object.fromEntries(
                Object.entries(data).sort(([, a], [, b]) => b - a)
            )
            for (let key in tmp) {
                if (parseInt(key) >= 49152) {
                    delete tmp[key]
                }
            }
            return tmp;
        }
    },
    mounted() {
        setInterval(() => {
            fetch('http://work.isk:8000/api/stats/tcp')
                .then(response => response.json())
                .then(data => {
                    this.tcpStats = data
                    this.tcpStats['srcPortCounter'] = this.preprocessData(this.tcpStats['srcPortCounter'])
                    this.tcpStats['dstPortCounter'] = this.preprocessData(this.tcpStats['dstPortCounter'])
                })
            fetch('http://work.isk:8000/api/stats/udp')
                .then(response => response.json())
                .then(data => {
                    this.udpStats = data
                    this.udpStats['srcPortCounter'] = this.preprocessData(this.udpStats['srcPortCounter'])
                    this.udpStats['dstPortCounter'] = this.preprocessData(this.udpStats['dstPortCounter'])
                })
        }, 1000)
    },
}
</script>
